# Music Player App

A mobile music player application built with Ionic Framework (React) that demonstrates master-detail UI, REST services, WebSockets, authentication, and offline capabilities.

## Features

### Core Functionality
- **Music Library Management**: Browse, search, and play music tracks
- **Master-Detail UI**: List view of all tracks with detailed view for each track
- **User Authentication**: Secure login/logout using JWT tokens
- **Real-time Updates**: WebSocket notifications for track updates from server

### Data Management
- **REST API Integration**: Fetch music tracks from REST services
- **Local Storage**: Offline data persistence using Ionic Storage
- **Pagination**: Infinite scrolling for efficient loading of large track lists
- **Search & Filter**: Find tracks by title, artist, album, or genre

### Network & Sync
- **Network Status Monitor**: Visual indicator showing online/offline status
- **Offline Support**:
  - Create and update tracks while offline
  - Local storage fallback when network unavailable
  - Track pending changes that haven't synced
- **Auto-Sync**: Automatically sync pending changes when connection restored
- **Optimistic Updates**: Try server first when online, fallback to local storage

### Security
- **JWT Authentication**: Token-based authentication
- **Persistent Login**: Store auth token in local storage
- **Auto-Login**: Skip login page if user already authenticated
- **Secured REST Services**: All API calls include authentication headers
- **Secured WebSockets**: WebSocket connections authenticated with JWT
- **User-Specific Data**: Each user sees only their own music library

## Technical Stack

- **Framework**: Ionic 7 with React
- **Language**: TypeScript
- **Storage**: @ionic/storage-angular (IndexedDB/LocalStorage)
- **HTTP Client**: Axios
- **WebSockets**: Socket.io-client
- **State Management**: React Context + Hooks

## Architecture

### Services
- `AuthService`: Handles login, logout, token management
- `StorageService`: Local data persistence
- `ApiService`: REST API communication with authentication
- `WebSocketService`: Real-time notifications
- `SyncService`: Offline/online data synchronization
- `NetworkService`: Network status monitoring

### Pages
- `LoginPage`: User authentication
- `MusicListPage`: Master view with pagination and search
- `MusicDetailPage`: Detail view for individual tracks
- `ProfilePage`: User profile and logout

## Data Model

```typescript
interface MusicTrack {
  id: string;
  title: string;
  artist: string;
  album: string;
  genre: string;
  duration: number;
  userId: string;
  createdAt: string;
  updatedAt: string;
  synced?: boolean; // Local flag for sync status
}
```

## Setup

1. Install dependencies:
```bash
npm install
```

2. Start the backend server:
```bash
cd server
npm install
npm start
```

3. Run the app:
```bash
ionic serve
```

## Backend API

The mock backend server provides:
- `POST /api/auth/login` - User authentication
- `GET /api/tracks` - Get user's tracks (paginated)
- `POST /api/tracks` - Create new track
- `PUT /api/tracks/:id` - Update track
- `DELETE /api/tracks/:id` - Delete track
- `WebSocket /` - Real-time notifications

## Test Credentials

- Username: `user1` / Password: `password1`
- Username: `user2` / Password: `password2`

## Assessment Requirements Checklist

- [x] Network status indicator (online/offline)
- [x] User authentication with JWT
- [x] Auth token stored in local storage
- [x] Auto-login if authenticated
- [x] Logout functionality
- [x] Resources linked to authenticated user
- [x] REST services return only user's resources
- [x] WebSocket notifications filtered by user
- [x] Online mode tries REST services first
- [x] Offline mode stores data locally
- [x] Pending changes indicator
- [x] Auto-sync when online
- [x] Pagination with infinite scroll
- [x] Search and filter functionality
