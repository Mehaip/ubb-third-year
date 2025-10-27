import { MusicTrack, SyncStatus } from '../types';
import ApiService from './ApiService';
import StorageService from './StorageService';
import NetworkService from './NetworkService';

type SyncStatusListener = (status: SyncStatus) => void;

class SyncService {
  private listeners: SyncStatusListener[] = [];
  private isSyncing: boolean = false;
  private lastSyncTime?: Date;

  async init(): Promise<void> {
    // Subscribe to network changes to auto-sync
    NetworkService.subscribe((status) => {
      if (status.isOnline && !this.isSyncing) {
        this.syncPendingChanges();
      }
    });
  }

  async syncPendingChanges(): Promise<void> {
    if (this.isSyncing || !NetworkService.isOnline()) {
      return;
    }

    this.isSyncing = true;
    this.notifyListeners();

    try {
      const pendingActions = await StorageService.getPendingActions();

      for (const track of pendingActions) {
        try {
          if (track.pendingAction === 'create') {
            const { id, synced, pendingAction, ...trackData } = track;
            const createdTrack = await ApiService.createTrack(trackData);

            // Update local storage with server ID
            await StorageService.deleteTrack(track.id);
            await StorageService.saveTrack({ ...createdTrack, synced: true });
            await StorageService.clearPendingAction(track.id);
          } else if (track.pendingAction === 'update') {
            const updatedTrack = await ApiService.updateTrack(track.id, track);
            await StorageService.saveTrack({ ...updatedTrack, synced: true });
            await StorageService.clearPendingAction(track.id);
          } else if (track.pendingAction === 'delete') {
            await ApiService.deleteTrack(track.id);
            await StorageService.deleteTrack(track.id);
            await StorageService.clearPendingAction(track.id);
          }
        } catch (error) {
          console.error(`Failed to sync track ${track.id}:`, error);
          // Continue with next track
        }
      }

      this.lastSyncTime = new Date();
    } catch (error) {
      console.error('Sync failed:', error);
    } finally {
      this.isSyncing = false;
      this.notifyListeners();
    }
  }

  async getPendingCount(): Promise<number> {
    const pending = await StorageService.getPendingActions();
    return pending.length;
  }

  subscribe(listener: SyncStatusListener): () => void {
    this.listeners.push(listener);

    // Return unsubscribe function
    return () => {
      this.listeners = this.listeners.filter(l => l !== listener);
    };
  }

  private notifyListeners(): void {
    this.getPendingCount().then(count => {
      const status: SyncStatus = {
        pendingCount: count,
        isSyncing: this.isSyncing,
        lastSyncTime: this.lastSyncTime
      };
      this.listeners.forEach(listener => listener(status));
    });
  }

  async createTrack(track: Omit<MusicTrack, 'id' | 'userId' | 'createdAt' | 'updatedAt'>): Promise<MusicTrack> {
    const newTrack: MusicTrack = {
      ...track,
      id: `temp_${Date.now()}_${Math.random()}`,
      userId: '',
      createdAt: new Date().toISOString(),
      updatedAt: new Date().toISOString(),
      synced: false,
      pendingAction: 'create'
    };

    if (NetworkService.isOnline()) {
      try {
        const createdTrack = await ApiService.createTrack(track);
        await StorageService.saveTrack({ ...createdTrack, synced: true });
        return createdTrack;
      } catch (error) {
        console.error('Failed to create track on server, saving locally:', error);
        await StorageService.saveTrack(newTrack);
        await StorageService.savePendingAction(newTrack);
        this.notifyListeners();
        return newTrack;
      }
    } else {
      await StorageService.saveTrack(newTrack);
      await StorageService.savePendingAction(newTrack);
      this.notifyListeners();
      return newTrack;
    }
  }

  async updateTrack(id: string, updates: Partial<MusicTrack>): Promise<MusicTrack> {
    const existingTrack = await StorageService.getTrack(id);
    if (!existingTrack) {
      throw new Error('Track not found');
    }

    const updatedTrack: MusicTrack = {
      ...existingTrack,
      ...updates,
      id,
      updatedAt: new Date().toISOString(),
      synced: false,
      pendingAction: 'update'
    };

    if (NetworkService.isOnline()) {
      try {
        const serverTrack = await ApiService.updateTrack(id, updates);
        await StorageService.saveTrack({ ...serverTrack, synced: true });
        return serverTrack;
      } catch (error) {
        console.error('Failed to update track on server, saving locally:', error);
        await StorageService.saveTrack(updatedTrack);
        await StorageService.savePendingAction(updatedTrack);
        this.notifyListeners();
        return updatedTrack;
      }
    } else {
      await StorageService.saveTrack(updatedTrack);
      await StorageService.savePendingAction(updatedTrack);
      this.notifyListeners();
      return updatedTrack;
    }
  }

  async deleteTrack(id: string): Promise<void> {
    const track = await StorageService.getTrack(id);
    if (!track) {
      return;
    }

    if (NetworkService.isOnline()) {
      try {
        await ApiService.deleteTrack(id);
        await StorageService.deleteTrack(id);
      } catch (error) {
        console.error('Failed to delete track on server, marking for deletion:', error);
        const deletedTrack = { ...track, pendingAction: 'delete' as const };
        await StorageService.savePendingAction(deletedTrack);
        this.notifyListeners();
      }
    } else {
      const deletedTrack = { ...track, pendingAction: 'delete' as const };
      await StorageService.savePendingAction(deletedTrack);
      this.notifyListeners();
    }
  }
}

export default new SyncService();
