# Setup Instructions

## Prerequisites

- Node.js (v16 or higher)
- npm

## Installation

### 1. Install App Dependencies

```bash
npm install
```

### 2. Install Server Dependencies

```bash
cd server
npm install
cd ..
```

## Running the Application

### Start the Backend Server

Open a terminal and run:

```bash
cd server
npm start
```

The server will start on http://localhost:3001

### Start the Ionic App

Open another terminal and run:

```bash
npm start
```

The app will start on http://localhost:3000

## Testing the Application

### Login Credentials

- User 1: `user1` / `password1` (has 25 tracks)
- User 2: `user2` / `password2` (has 5 tracks)

### Features to Test

1. **Authentication**
   - Login with credentials
   - App should remember you on refresh (JWT stored in localStorage)
   - Logout functionality

2. **Network Status**
   - Online/offline indicator in header
   - Test by turning off server or using browser dev tools (Network tab > Offline)

3. **Music List (Master View)**
   - View all tracks with pagination
   - Scroll down to trigger infinite scroll (loads next page)
   - Pull down to refresh

4. **Search & Filter**
   - Use search bar to find tracks by title, artist, or album
   - Use genre dropdown to filter by genre

5. **Track Detail (Detail View)**
   - Click any track to view details
   - Edit track information
   - Delete tracks
   - Create new tracks (click + button)

6. **Offline Mode**
   - Turn off the server
   - Notice offline indicator appears
   - Create/edit tracks (they'll be saved locally)
   - Notice "Not Synced" badge on tracks
   - Notice pending count badge in header

7. **Auto-Sync**
   - Turn server back on
   - App should automatically sync pending changes
   - Click sync button if needed
   - Pending count should go to 0

8. **WebSocket Notifications**
   - Login as both users in different browsers
   - Create/edit/delete a track as one user
   - Changes should appear in real-time for that user only

9. **User Isolation**
   - Login as user1 - see 25 tracks
   - Logout and login as user2 - see only 5 different tracks
   - Each user only sees their own data

## Architecture

```
app/
├── src/
│   ├── context/          # React Context for global state
│   ├── pages/            # Page components (Login, List, Detail)
│   ├── services/         # Business logic services
│   │   ├── AuthService.ts
│   │   ├── StorageService.ts
│   │   ├── ApiService.ts
│   │   ├── WebSocketService.ts
│   │   ├── NetworkService.ts
│   │   └── SyncService.ts
│   ├── types/            # TypeScript interfaces
│   ├── App.tsx           # Main app with routing
│   └── main.tsx          # Entry point
└── server/
    └── server.js         # Express + Socket.io backend
```

## Key Implementation Details

### Authentication (1p)
- JWT tokens issued on login
- Token stored in localStorage
- Auto-login on app start if token exists
- Protected routes redirect to login if not authenticated

### Network Status (1p)
- Visual indicator (green = online, red = offline)
- Uses browser online/offline events

### User-Linked Resources (1p)
- REST endpoints filter by userId from JWT
- WebSocket notifications sent only to resource owner
- Each user has isolated data

### Online/Offline Behavior (2p)
- Online: REST API calls first, fallback to local storage on error
- Offline: All changes stored locally with pending flags
- Pending changes indicator shows unsync'd items

### Auto-Sync (1p)
- Monitors network status changes
- Automatically syncs when coming online
- Manual sync button also available

### Pagination (2p)
- Infinite scroll with IonInfiniteScroll
- Page size: 20 items
- Loads next page when scrolling to bottom

### Search & Filter (1p)
- Search by title, artist, or album
- Filter by genre dropdown
- Works both online and offline

## Troubleshooting

- **Port already in use**: Change port in `vite.config.ts` (app) or `server/server.js` (server)
- **CORS errors**: Ensure server is running on port 3001
- **WebSocket not connecting**: Check auth token is valid and server is running
- **Pagination not working**: Ensure you have enough data (user1 has 25 tracks)
