import { NetworkStatus } from '../types';

type NetworkStatusListener = (status: NetworkStatus) => void;

class NetworkService {
  private listeners: NetworkStatusListener[] = [];
  private currentStatus: NetworkStatus = {
    isOnline: navigator.onLine,
    lastChecked: new Date()
  };

  init(): void {
    window.addEventListener('online', this.handleOnline);
    window.addEventListener('offline', this.handleOffline);
  }

  private handleOnline = (): void => {
    this.currentStatus = {
      isOnline: true,
      lastChecked: new Date()
    };
    this.notifyListeners();
  };

  private handleOffline = (): void => {
    this.currentStatus = {
      isOnline: false,
      lastChecked: new Date()
    };
    this.notifyListeners();
  };

  private notifyListeners(): void {
    this.listeners.forEach(listener => listener(this.currentStatus));
  }

  subscribe(listener: NetworkStatusListener): () => void {
    this.listeners.push(listener);

    // Return unsubscribe function
    return () => {
      this.listeners = this.listeners.filter(l => l !== listener);
    };
  }

  getStatus(): NetworkStatus {
    return this.currentStatus;
  }

  isOnline(): boolean {
    return this.currentStatus.isOnline;
  }

  cleanup(): void {
    window.removeEventListener('online', this.handleOnline);
    window.removeEventListener('offline', this.handleOffline);
    this.listeners = [];
  }
}

export default new NetworkService();
