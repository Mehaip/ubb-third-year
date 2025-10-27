import React, { useState, useEffect } from 'react';
import {
  IonPage, IonHeader, IonToolbar, IonTitle, IonContent, IonList, IonItem,
  IonLabel, IonButton, IonButtons, IonSearchbar, IonSelect, IonSelectOption,
  IonInfiniteScroll, IonInfiniteScrollContent, IonChip, IonIcon, IonBadge,
  IonRefresher, IonRefresherContent, IonFab, IonFabButton
} from '@ionic/react';
import { useHistory } from 'react-router-dom';
import { logOutOutline, cloudOfflineOutline, cloudDoneOutline, add, syncOutline } from 'ionicons/icons';
import { MusicTrack } from '../types';
import ApiService from '../services/ApiService';
import StorageService from '../services/StorageService';
import AuthService from '../services/AuthService';
import WebSocketService from '../services/WebSocketService';
import SyncService from '../services/SyncService';
import NetworkService from '../services/NetworkService';
import { useApp } from '../context/AppContext';

const MusicListPage: React.FC = () => {
  const [tracks, setTracks] = useState<MusicTrack[]>([]);
  const [page, setPage] = useState(1);
  const [hasMore, setHasMore] = useState(true);
  const [search, setSearch] = useState('');
  const [genreFilter, setGenreFilter] = useState('');
  const [loading, setLoading] = useState(false);
  const history = useHistory();
  const { networkStatus, syncStatus, setUser } = useApp();

  const genres = ['Rock', 'Pop', 'Jazz', 'Classical', 'Electronic', 'Hip Hop'];

  useEffect(() => {
    loadTracks(1, true);

    // Subscribe to WebSocket updates
    const unsubscribe = WebSocketService.subscribe((message) => {
      if (message.type === 'track_created') {
        setTracks(prev => [message.data, ...prev]);
      } else if (message.type === 'track_updated') {
        setTracks(prev => prev.map(t => t.id === message.data.id ? message.data : t));
      } else if (message.type === 'track_deleted') {
        setTracks(prev => prev.filter(t => t.id !== message.data.id));
      }
    });

    return () => unsubscribe();
  }, []);

  const loadTracks = async (pageNum: number, reset: boolean = false) => {
    if (loading) return;

    setLoading(true);
    try {
      if (NetworkService.isOnline()) {
        const response = await ApiService.getTracks(pageNum, 20, search, genreFilter);
        const newTracks = reset ? response.items : [...tracks, ...response.items];
        setTracks(newTracks);
        setHasMore(response.hasMore);
        setPage(pageNum);

        // Save to local storage
        await StorageService.saveTracks(newTracks);
      } else {
        // Load from local storage
        const localTracks = await StorageService.getTracks();
        let filtered = localTracks;

        if (search) {
          filtered = filtered.filter(t =>
            t.title.toLowerCase().includes(search.toLowerCase()) ||
            t.artist.toLowerCase().includes(search.toLowerCase()) ||
            t.album.toLowerCase().includes(search.toLowerCase())
          );
        }

        if (genreFilter) {
          filtered = filtered.filter(t => t.genre === genreFilter);
        }

        setTracks(filtered);
        setHasMore(false);
      }
    } catch (error) {
      console.error('Failed to load tracks:', error);
      // Fallback to local storage
      const localTracks = await StorageService.getTracks();
      setTracks(localTracks);
      setHasMore(false);
    } finally {
      setLoading(false);
    }
  };

  const handleSearch = () => {
    loadTracks(1, true);
  };

  const handleRefresh = async (event: any) => {
    await loadTracks(1, true);
    event.detail.complete();
  };

  const loadMore = (event: any) => {
    loadTracks(page + 1, false).then(() => {
      event.target.complete();
    });
  };

  const handleLogout = async () => {
    await AuthService.logout();
    WebSocketService.disconnect();
    setUser(null);
    history.replace('/login');
  };

  const handleSync = async () => {
    await SyncService.syncPendingChanges();
    await loadTracks(1, true);
  };

  const formatDuration = (seconds: number): string => {
    const mins = Math.floor(seconds / 60);
    const secs = seconds % 60;
    return `${mins}:${secs.toString().padStart(2, '0')}`;
  };

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar>
          <IonTitle>Music Library</IonTitle>
          <IonButtons slot="end">
            {networkStatus.isOnline ? (
              <IonChip color="success">
                <IonIcon icon={cloudDoneOutline} />
                <IonLabel>Online</IonLabel>
              </IonChip>
            ) : (
              <IonChip color="danger">
                <IonIcon icon={cloudOfflineOutline} />
                <IonLabel>Offline</IonLabel>
              </IonChip>
            )}
            {syncStatus.pendingCount > 0 && (
              <IonButton onClick={handleSync} disabled={syncStatus.isSyncing}>
                <IonIcon icon={syncOutline} slot="icon-only" />
                <IonBadge color="warning">{syncStatus.pendingCount}</IonBadge>
              </IonButton>
            )}
            <IonButton onClick={handleLogout}>
              <IonIcon icon={logOutOutline} slot="icon-only" />
            </IonButton>
          </IonButtons>
        </IonToolbar>
        <IonToolbar>
          <IonSearchbar
            value={search}
            onIonInput={(e) => setSearch(e.detail.value!)}
            onIonChange={handleSearch}
            placeholder="Search tracks, artists, albums..."
          />
        </IonToolbar>
        <IonToolbar>
          <IonSelect
            value={genreFilter}
            placeholder="Filter by genre"
            onIonChange={(e) => {
              setGenreFilter(e.detail.value);
              setTimeout(() => loadTracks(1, true), 100);
            }}
          >
            <IonSelectOption value="">All Genres</IonSelectOption>
            {genres.map(genre => (
              <IonSelectOption key={genre} value={genre}>{genre}</IonSelectOption>
            ))}
          </IonSelect>
        </IonToolbar>
      </IonHeader>

      <IonContent>
        <IonRefresher slot="fixed" onIonRefresh={handleRefresh}>
          <IonRefresherContent />
        </IonRefresher>

        <IonList>
          {tracks.map(track => (
            <IonItem
              key={track.id}
              button
              onClick={() => history.push(`/tracks/${track.id}`)}
            >
              <IonLabel>
                <h2>{track.title}</h2>
                <p>{track.artist} - {track.album}</p>
                <p>{track.genre} • {formatDuration(track.duration)}</p>
              </IonLabel>
              {!track.synced && (
                <IonChip color="warning" slot="end">
                  <IonLabel>Not Synced</IonLabel>
                </IonChip>
              )}
            </IonItem>
          ))}
        </IonList>

        <IonInfiniteScroll
          onIonInfinite={loadMore}
          threshold="100px"
          disabled={!hasMore || !networkStatus.isOnline}
        >
          <IonInfiniteScrollContent loadingText="Loading more tracks..." />
        </IonInfiniteScroll>

        <IonFab vertical="bottom" horizontal="end" slot="fixed">
          <IonFabButton onClick={() => history.push('/tracks/new')}>
            <IonIcon icon={add} />
          </IonFabButton>
        </IonFab>
      </IonContent>
    </IonPage>
  );
};

export default MusicListPage;
