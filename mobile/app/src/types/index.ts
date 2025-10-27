export interface MusicTrack {
  id: string;
  title: string;
  artist: string;
  album: string;
  genre: string;
  duration: number; // in seconds
  userId: string;
  createdAt: string;
  updatedAt: string;
  synced?: boolean; // Local flag indicating if synced to server
  pendingAction?: 'create' | 'update' | 'delete'; // Pending action for offline changes
}

export interface User {
  id: string;
  username: string;
  token: string;
}

export interface LoginCredentials {
  username: string;
  password: string;
}

export interface AuthResponse {
  token: string;
  user: {
    id: string;
    username: string;
  };
}

export interface PaginatedResponse<T> {
  items: T[];
  total: number;
  page: number;
  pageSize: number;
  hasMore: boolean;
}

export interface WebSocketMessage {
  type: 'track_created' | 'track_updated' | 'track_deleted';
  data: MusicTrack;
  userId: string;
}

export interface NetworkStatus {
  isOnline: boolean;
  lastChecked: Date;
}

export interface SyncStatus {
  pendingCount: number;
  isSyncing: boolean;
  lastSyncTime?: Date;
}
