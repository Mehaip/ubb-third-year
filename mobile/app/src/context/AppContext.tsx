import React, { createContext, useContext, useState, useEffect, ReactNode } from 'react';
import { User, NetworkStatus, SyncStatus } from '../types';
import AuthService from '../services/AuthService';
import NetworkService from '../services/NetworkService';
import SyncService from '../services/SyncService';
import WebSocketService from '../services/WebSocketService';

interface AppContextType {
  user: User | null;
  setUser: (user: User | null) => void;
  networkStatus: NetworkStatus;
  syncStatus: SyncStatus;
  isAuthenticated: boolean;
}

const AppContext = createContext<AppContextType | undefined>(undefined);

export const AppProvider: React.FC<{ children: ReactNode }> = ({ children }) => {
  const [user, setUser] = useState<User | null>(null);
  const [networkStatus, setNetworkStatus] = useState<NetworkStatus>(NetworkService.getStatus());
  const [syncStatus, setSyncStatus] = useState<SyncStatus>({
    pendingCount: 0,
    isSyncing: false
  });

  useEffect(() => {
    // Initialize services
    const initServices = async () => {
      await AuthService.init();
      NetworkService.init();
      await SyncService.init();

      const currentUser = AuthService.getCurrentUser();
      setUser(currentUser);

      if (currentUser) {
        WebSocketService.connect();
      }
    };

    initServices();

    // Subscribe to network status changes
    const unsubscribeNetwork = NetworkService.subscribe(setNetworkStatus);

    // Subscribe to sync status changes
    const unsubscribeSync = SyncService.subscribe(setSyncStatus);

    return () => {
      unsubscribeNetwork();
      unsubscribeSync();
      NetworkService.cleanup();
      WebSocketService.disconnect();
    };
  }, []);

  const value: AppContextType = {
    user,
    setUser,
    networkStatus,
    syncStatus,
    isAuthenticated: user !== null
  };

  return <AppContext.Provider value={value}>{children}</AppContext.Provider>;
};

export const useApp = (): AppContextType => {
  const context = useContext(AppContext);
  if (context === undefined) {
    throw new Error('useApp must be used within an AppProvider');
  }
  return context;
};
