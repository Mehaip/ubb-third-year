import React, { useState, useEffect } from 'react';
import {
  IonPage, IonHeader, IonToolbar, IonTitle, IonContent, IonBackButton,
  IonButtons, IonButton, IonItem, IonLabel, IonInput, IonSelect,
  IonSelectOption, IonLoading, IonAlert
} from '@ionic/react';
import { useHistory, useParams } from 'react-router-dom';
import { MusicTrack } from '../types';
import ApiService from '../services/ApiService';
import StorageService from '../services/StorageService';
import SyncService from '../services/SyncService';
import NetworkService from '../services/NetworkService';

const MusicDetailPage: React.FC = () => {
  const { id } = useParams<{ id: string }>();
  const history = useHistory();
  const isNew = id === 'new';

  const [track, setTrack] = useState<Partial<MusicTrack>>({
    title: '',
    artist: '',
    album: '',
    genre: 'Rock',
    duration: 0
  });
  const [loading, setLoading] = useState(false);
  const [showDeleteAlert, setShowDeleteAlert] = useState(false);

  const genres = ['Rock', 'Pop', 'Jazz', 'Classical', 'Electronic', 'Hip Hop'];

  useEffect(() => {
    if (!isNew) {
      loadTrack();
    }
  }, [id]);

  const loadTrack = async () => {
    setLoading(true);
    try {
      if (NetworkService.isOnline()) {
        const loadedTrack = await ApiService.getTrack(id);
        setTrack(loadedTrack);
      } else {
        const localTrack = await StorageService.getTrack(id);
        if (localTrack) {
          setTrack(localTrack);
        }
      }
    } catch (error) {
      console.error('Failed to load track:', error);
      const localTrack = await StorageService.getTrack(id);
      if (localTrack) {
        setTrack(localTrack);
      }
    } finally {
      setLoading(false);
    }
  };

  const handleSave = async () => {
    if (!track.title || !track.artist || !track.album || !track.duration) {
      return;
    }

    setLoading(true);
    try {
      if (isNew) {
        await SyncService.createTrack({
          title: track.title,
          artist: track.artist,
          album: track.album,
          genre: track.genre || 'Rock',
          duration: track.duration
        });
      } else {
        await SyncService.updateTrack(id, track);
      }
      history.goBack();
    } catch (error) {
      console.error('Failed to save track:', error);
    } finally {
      setLoading(false);
    }
  };

  const handleDelete = async () => {
    setLoading(true);
    try {
      await SyncService.deleteTrack(id);
      history.replace('/tracks');
    } catch (error) {
      console.error('Failed to delete track:', error);
    } finally {
      setLoading(false);
    }
  };

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar>
          <IonButtons slot="start">
            <IonBackButton defaultHref="/tracks" />
          </IonButtons>
          <IonTitle>{isNew ? 'New Track' : 'Edit Track'}</IonTitle>
          <IonButtons slot="end">
            {!isNew && (
              <IonButton color="danger" onClick={() => setShowDeleteAlert(true)}>
                Delete
              </IonButton>
            )}
            <IonButton onClick={handleSave}>
              Save
            </IonButton>
          </IonButtons>
        </IonToolbar>
      </IonHeader>

      <IonContent className="ion-padding">
        <IonLoading isOpen={loading} message="Please wait..." />

        <IonItem>
          <IonLabel position="floating">Title *</IonLabel>
          <IonInput
            value={track.title}
            onIonInput={(e) => setTrack({ ...track, title: e.detail.value! })}
            required
          />
        </IonItem>

        <IonItem>
          <IonLabel position="floating">Artist *</IonLabel>
          <IonInput
            value={track.artist}
            onIonInput={(e) => setTrack({ ...track, artist: e.detail.value! })}
            required
          />
        </IonItem>

        <IonItem>
          <IonLabel position="floating">Album *</IonLabel>
          <IonInput
            value={track.album}
            onIonInput={(e) => setTrack({ ...track, album: e.detail.value! })}
            required
          />
        </IonItem>

        <IonItem>
          <IonLabel>Genre</IonLabel>
          <IonSelect
            value={track.genre}
            onIonChange={(e) => setTrack({ ...track, genre: e.detail.value })}
          >
            {genres.map(genre => (
              <IonSelectOption key={genre} value={genre}>{genre}</IonSelectOption>
            ))}
          </IonSelect>
        </IonItem>

        <IonItem>
          <IonLabel position="floating">Duration (seconds) *</IonLabel>
          <IonInput
            type="number"
            value={track.duration}
            onIonInput={(e) => setTrack({ ...track, duration: parseInt(e.detail.value!) || 0 })}
            required
          />
        </IonItem>

        {!isNew && track.synced === false && (
          <div style={{ padding: '20px', color: '#ff9800' }}>
            This track has not been synced to the server yet.
          </div>
        )}

        <IonAlert
          isOpen={showDeleteAlert}
          onDidDismiss={() => setShowDeleteAlert(false)}
          header="Delete Track"
          message="Are you sure you want to delete this track?"
          buttons={[
            {
              text: 'Cancel',
              role: 'cancel'
            },
            {
              text: 'Delete',
              role: 'destructive',
              handler: handleDelete
            }
          ]}
        />
      </IonContent>
    </IonPage>
  );
};

export default MusicDetailPage;
