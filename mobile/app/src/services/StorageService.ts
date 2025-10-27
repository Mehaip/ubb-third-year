import { MusicTrack, User } from '../types';

class StorageService {
  private readonly TRACKS_KEY = 'music_tracks';
  private readonly AUTH_TOKEN_KEY = 'auth_token';
  private readonly USER_KEY = 'user';
  private readonly PENDING_ACTIONS_KEY = 'pending_actions';

  async init(): Promise<void> {
    // Storage is ready
  }

  // Auth related
  async saveAuthToken(token: string): Promise<void> {
    localStorage.setItem(this.AUTH_TOKEN_KEY, token);
  }

  async getAuthToken(): Promise<string | null> {
    return localStorage.getItem(this.AUTH_TOKEN_KEY);
  }

  async saveUser(user: User): Promise<void> {
    localStorage.setItem(this.USER_KEY, JSON.stringify(user));
  }

  async getUser(): Promise<User | null> {
    const userStr = localStorage.getItem(this.USER_KEY);
    return userStr ? JSON.parse(userStr) : null;
  }

  async clearAuth(): Promise<void> {
    localStorage.removeItem(this.AUTH_TOKEN_KEY);
    localStorage.removeItem(this.USER_KEY);
  }

  // Track related
  async saveTracks(tracks: MusicTrack[]): Promise<void> {
    localStorage.setItem(this.TRACKS_KEY, JSON.stringify(tracks));
  }

  async getTracks(): Promise<MusicTrack[]> {
    const tracksStr = localStorage.getItem(this.TRACKS_KEY);
    return tracksStr ? JSON.parse(tracksStr) : [];
  }

  async saveTrack(track: MusicTrack): Promise<void> {
    const tracks = await this.getTracks();
    const index = tracks.findIndex(t => t.id === track.id);

    if (index >= 0) {
      tracks[index] = track;
    } else {
      tracks.push(track);
    }

    await this.saveTracks(tracks);
  }

  async deleteTrack(trackId: string): Promise<void> {
    const tracks = await this.getTracks();
    const filtered = tracks.filter(t => t.id !== trackId);
    await this.saveTracks(filtered);
  }

  async getTrack(trackId: string): Promise<MusicTrack | null> {
    const tracks = await this.getTracks();
    return tracks.find(t => t.id === trackId) || null;
  }

  // Pending actions for offline sync
  async savePendingAction(track: MusicTrack): Promise<void> {
    const pendingStr = localStorage.getItem(this.PENDING_ACTIONS_KEY);
    const pending: MusicTrack[] = pendingStr ? JSON.parse(pendingStr) : [];

    // Remove existing action for same track
    const filtered = pending.filter(t => t.id !== track.id);

    // Add new action (unless it's a delete of a track that was created offline)
    if (track.pendingAction === 'delete' && pending.find(t => t.id === track.id && t.pendingAction === 'create')) {
      // Just remove from pending
    } else {
      filtered.push(track);
    }

    localStorage.setItem(this.PENDING_ACTIONS_KEY, JSON.stringify(filtered));
  }

  async getPendingActions(): Promise<MusicTrack[]> {
    const pendingStr = localStorage.getItem(this.PENDING_ACTIONS_KEY);
    return pendingStr ? JSON.parse(pendingStr) : [];
  }

  async clearPendingAction(trackId: string): Promise<void> {
    const pending = await this.getPendingActions();
    const filtered = pending.filter(t => t.id !== trackId);
    localStorage.setItem(this.PENDING_ACTIONS_KEY, JSON.stringify(filtered));
  }

  async clearAllPendingActions(): Promise<void> {
    localStorage.removeItem(this.PENDING_ACTIONS_KEY);
  }

  async clearAll(): Promise<void> {
    localStorage.clear();
  }
}

export default new StorageService();
