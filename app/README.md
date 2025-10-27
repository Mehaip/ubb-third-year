# Music Player App

Minimal mobile app runnable on laptop with all required features.

## Features

### Core Requirements
- **Master-Detail UI**: List of songs on left, detail/edit form on right
- **REST API**: Fetches songs from server with pagination
- **WebSockets**: Real-time notifications for song changes
- **Local Storage**: Caches songs and auth token
- **Pagination**: Load more button for infinite scrolling
- **Search/Filter**: Search by title or artist

### Authentication
- JWT-based authentication
- Login page shown if not authenticated
- Token stored in localStorage
- Auto-login on app start if token exists
- Logout functionality
- User-specific resources (songs linked to user)

### Online/Offline Behavior
- Network status indicator (green=online, red=offline)
- In online mode: tries REST API first
- In offline mode: stores operations locally
- Shows count of pending operations
- Auto-syncs when back online
- Failed REST calls saved for retry

## How to Run

1. Install dependencies:
```bash
npm install
```

2. Start backend server (terminal 1):
```bash
npm run server
```

3. Start React app (terminal 2):
```bash
npm start
```

4. Open http://localhost:3000 in browser

## Login Credentials
- user1 / pass1
- user2 / pass2

## Tech Stack
- React (UI)
- Express (REST API)
- Socket.io (WebSockets)
- JWT (Authentication)
- LocalStorage (Offline storage)
- Axios (HTTP client)
