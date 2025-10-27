import { io, Socket } from 'socket.io-client';
import { WebSocketMessage } from '../types';
import AuthService from './AuthService';

const WS_URL = 'http://localhost:3001';

type MessageListener = (message: WebSocketMessage) => void;

class WebSocketService {
  private socket: Socket | null = null;
  private listeners: MessageListener[] = [];
  private connected: boolean = false;

  connect(): void {
    const token = AuthService.getAuthToken();
    if (!token) {
      console.warn('Cannot connect WebSocket: No auth token');
      return;
    }

    this.socket = io(WS_URL, {
      auth: {
        token
      },
      reconnection: true,
      reconnectionDelay: 1000,
      reconnectionAttempts: 5
    });

    this.socket.on('connect', () => {
      console.log('WebSocket connected');
      this.connected = true;
    });

    this.socket.on('disconnect', () => {
      console.log('WebSocket disconnected');
      this.connected = false;
    });

    this.socket.on('track_created', (data: WebSocketMessage) => {
      this.notifyListeners(data);
    });

    this.socket.on('track_updated', (data: WebSocketMessage) => {
      this.notifyListeners(data);
    });

    this.socket.on('track_deleted', (data: WebSocketMessage) => {
      this.notifyListeners(data);
    });

    this.socket.on('connect_error', (error) => {
      console.error('WebSocket connection error:', error);
    });
  }

  disconnect(): void {
    if (this.socket) {
      this.socket.disconnect();
      this.socket = null;
      this.connected = false;
    }
  }

  subscribe(listener: MessageListener): () => void {
    this.listeners.push(listener);

    // Return unsubscribe function
    return () => {
      this.listeners = this.listeners.filter(l => l !== listener);
    };
  }

  private notifyListeners(message: WebSocketMessage): void {
    // Only notify if message is for current user
    const currentUser = AuthService.getCurrentUser();
    if (currentUser && message.userId === currentUser.id) {
      this.listeners.forEach(listener => listener(message));
    }
  }

  isConnected(): boolean {
    return this.connected;
  }
}

export default new WebSocketService();
