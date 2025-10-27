const express = require('express');
const http = require('http');
const socketIO = require('socket.io');
const cors = require('cors');
const bodyParser = require('body-parser');
const jwt = require('jsonwebtoken');

const app = express();
const server = http.createServer(app);
const io = socketIO(server, {
  cors: {
    origin: '*',
    methods: ['GET', 'POST']
  }
});

const PORT = 3001;
const JWT_SECRET = 'music-player-secret-key';

app.use(cors());
app.use(bodyParser.json());

// Mock database
const users = [
  { id: '1', username: 'user1', password: 'password1' },
  { id: '2', username: 'user2', password: 'password2' }
];

let tracks = [
  {
    id: '1',
    title: 'Bohemian Rhapsody',
    artist: 'Queen',
    album: 'A Night at the Opera',
    genre: 'Rock',
    duration: 354,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '2',
    title: 'Stairway to Heaven',
    artist: 'Led Zeppelin',
    album: 'Led Zeppelin IV',
    genre: 'Rock',
    duration: 482,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '3',
    title: 'Imagine',
    artist: 'John Lennon',
    album: 'Imagine',
    genre: 'Pop',
    duration: 183,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '4',
    title: 'Hotel California',
    artist: 'Eagles',
    album: 'Hotel California',
    genre: 'Rock',
    duration: 391,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '5',
    title: 'Sweet Child O Mine',
    artist: 'Guns N Roses',
    album: 'Appetite for Destruction',
    genre: 'Rock',
    duration: 356,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '6',
    title: 'Smells Like Teen Spirit',
    artist: 'Nirvana',
    album: 'Nevermind',
    genre: 'Rock',
    duration: 301,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '7',
    title: 'Thriller',
    artist: 'Michael Jackson',
    album: 'Thriller',
    genre: 'Pop',
    duration: 357,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '8',
    title: 'Beat It',
    artist: 'Michael Jackson',
    album: 'Thriller',
    genre: 'Pop',
    duration: 258,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '9',
    title: 'Like a Prayer',
    artist: 'Madonna',
    album: 'Like a Prayer',
    genre: 'Pop',
    duration: 340,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '10',
    title: 'Purple Rain',
    artist: 'Prince',
    album: 'Purple Rain',
    genre: 'Pop',
    duration: 498,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '11',
    title: 'So What',
    artist: 'Miles Davis',
    album: 'Kind of Blue',
    genre: 'Jazz',
    duration: 562,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '12',
    title: 'Take Five',
    artist: 'Dave Brubeck',
    album: 'Time Out',
    genre: 'Jazz',
    duration: 324,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '13',
    title: 'Summertime',
    artist: 'Ella Fitzgerald',
    album: 'Porgy and Bess',
    genre: 'Jazz',
    duration: 278,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '14',
    title: 'Symphony No. 5',
    artist: 'Beethoven',
    album: 'Beethoven Symphonies',
    genre: 'Classical',
    duration: 1980,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '15',
    title: 'Four Seasons - Spring',
    artist: 'Vivaldi',
    album: 'The Four Seasons',
    genre: 'Classical',
    duration: 625,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '16',
    title: 'Blue Monday',
    artist: 'New Order',
    album: 'Power, Corruption & Lies',
    genre: 'Electronic',
    duration: 442,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '17',
    title: 'Sandstorm',
    artist: 'Darude',
    album: 'Before the Storm',
    genre: 'Electronic',
    duration: 228,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '18',
    title: 'Around the World',
    artist: 'Daft Punk',
    album: 'Homework',
    genre: 'Electronic',
    duration: 429,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '19',
    title: 'Lose Yourself',
    artist: 'Eminem',
    album: '8 Mile Soundtrack',
    genre: 'Hip Hop',
    duration: 326,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '20',
    title: 'Juicy',
    artist: 'The Notorious B.I.G.',
    album: 'Ready to Die',
    genre: 'Hip Hop',
    duration: 301,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '21',
    title: 'N.Y. State of Mind',
    artist: 'Nas',
    album: 'Illmatic',
    genre: 'Hip Hop',
    duration: 295,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '22',
    title: 'The Message',
    artist: 'Grandmaster Flash',
    album: 'The Message',
    genre: 'Hip Hop',
    duration: 442,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '23',
    title: 'Comfortably Numb',
    artist: 'Pink Floyd',
    album: 'The Wall',
    genre: 'Rock',
    duration: 382,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '24',
    title: 'November Rain',
    artist: 'Guns N Roses',
    album: 'Use Your Illusion I',
    genre: 'Rock',
    duration: 537,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '25',
    title: 'Dream On',
    artist: 'Aerosmith',
    album: 'Aerosmith',
    genre: 'Rock',
    duration: 267,
    userId: '1',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '26',
    title: 'Billie Jean',
    artist: 'Michael Jackson',
    album: 'Thriller',
    genre: 'Pop',
    duration: 294,
    userId: '2',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '27',
    title: 'Take Five',
    artist: 'Dave Brubeck',
    album: 'Time Out',
    genre: 'Jazz',
    duration: 324,
    userId: '2',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '28',
    title: 'Shape of You',
    artist: 'Ed Sheeran',
    album: 'Divide',
    genre: 'Pop',
    duration: 233,
    userId: '2',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '29',
    title: 'Blinding Lights',
    artist: 'The Weeknd',
    album: 'After Hours',
    genre: 'Pop',
    duration: 200,
    userId: '2',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  },
  {
    id: '30',
    title: 'Wonderwall',
    artist: 'Oasis',
    album: 'What\'s the Story Morning Glory',
    genre: 'Rock',
    duration: 258,
    userId: '2',
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  }
];

let trackIdCounter = 31;

// Middleware to verify JWT token
const authenticateToken = (req, res, next) => {
  const authHeader = req.headers['authorization'];
  const token = authHeader && authHeader.split(' ')[1];

  if (!token) {
    return res.status(401).json({ error: 'No token provided' });
  }

  jwt.verify(token, JWT_SECRET, (err, user) => {
    if (err) {
      return res.status(403).json({ error: 'Invalid token' });
    }
    req.user = user;
    next();
  });
};

// Auth endpoints
app.post('/api/auth/login', (req, res) => {
  const { username, password } = req.body;

  const user = users.find(u => u.username === username && u.password === password);

  if (!user) {
    return res.status(401).json({ error: 'Invalid credentials' });
  }

  const token = jwt.sign({ id: user.id, username: user.username }, JWT_SECRET);

  res.json({
    token,
    user: {
      id: user.id,
      username: user.username
    }
  });
});

// Track endpoints
app.get('/api/tracks', authenticateToken, (req, res) => {
  const { page = 1, pageSize = 20, search = '', genre = '' } = req.query;
  const userId = req.user.id;

  // Filter tracks by user
  let userTracks = tracks.filter(t => t.userId === userId);

  // Apply search filter
  if (search) {
    const searchLower = search.toLowerCase();
    userTracks = userTracks.filter(t =>
      t.title.toLowerCase().includes(searchLower) ||
      t.artist.toLowerCase().includes(searchLower) ||
      t.album.toLowerCase().includes(searchLower)
    );
  }

  // Apply genre filter
  if (genre) {
    userTracks = userTracks.filter(t => t.genre === genre);
  }

  // Pagination
  const pageNum = parseInt(page);
  const size = parseInt(pageSize);
  const startIndex = (pageNum - 1) * size;
  const endIndex = startIndex + size;

  const paginatedTracks = userTracks.slice(startIndex, endIndex);
  const hasMore = endIndex < userTracks.length;

  res.json({
    items: paginatedTracks,
    total: userTracks.length,
    page: pageNum,
    pageSize: size,
    hasMore
  });
});

app.get('/api/tracks/:id', authenticateToken, (req, res) => {
  const { id } = req.params;
  const userId = req.user.id;

  const track = tracks.find(t => t.id === id && t.userId === userId);

  if (!track) {
    return res.status(404).json({ error: 'Track not found' });
  }

  res.json(track);
});

app.post('/api/tracks', authenticateToken, (req, res) => {
  const userId = req.user.id;
  const { title, artist, album, genre, duration } = req.body;

  const newTrack = {
    id: String(trackIdCounter++),
    title,
    artist,
    album,
    genre,
    duration,
    userId,
    createdAt: new Date().toISOString(),
    updatedAt: new Date().toISOString()
  };

  tracks.push(newTrack);

  // Emit WebSocket event to user
  io.to(`user:${userId}`).emit('track_created', {
    type: 'track_created',
    data: newTrack,
    userId
  });

  res.status(201).json(newTrack);
});

app.put('/api/tracks/:id', authenticateToken, (req, res) => {
  const { id } = req.params;
  const userId = req.user.id;
  const updates = req.body;

  const trackIndex = tracks.findIndex(t => t.id === id && t.userId === userId);

  if (trackIndex === -1) {
    return res.status(404).json({ error: 'Track not found' });
  }

  const updatedTrack = {
    ...tracks[trackIndex],
    ...updates,
    id,
    userId,
    updatedAt: new Date().toISOString()
  };

  tracks[trackIndex] = updatedTrack;

  // Emit WebSocket event to user
  io.to(`user:${userId}`).emit('track_updated', {
    type: 'track_updated',
    data: updatedTrack,
    userId
  });

  res.json(updatedTrack);
});

app.delete('/api/tracks/:id', authenticateToken, (req, res) => {
  const { id } = req.params;
  const userId = req.user.id;

  const trackIndex = tracks.findIndex(t => t.id === id && t.userId === userId);

  if (trackIndex === -1) {
    return res.status(404).json({ error: 'Track not found' });
  }

  const deletedTrack = tracks[trackIndex];
  tracks = tracks.filter(t => t.id !== id);

  // Emit WebSocket event to user
  io.to(`user:${userId}`).emit('track_deleted', {
    type: 'track_deleted',
    data: deletedTrack,
    userId
  });

  res.status(204).send();
});

// WebSocket authentication and connection
io.use((socket, next) => {
  const token = socket.handshake.auth.token;

  if (!token) {
    return next(new Error('Authentication error'));
  }

  jwt.verify(token, JWT_SECRET, (err, user) => {
    if (err) {
      return next(new Error('Authentication error'));
    }
    socket.user = user;
    next();
  });
});

io.on('connection', (socket) => {
  console.log(`User connected: ${socket.user.username} (${socket.user.id})`);

  // Join user-specific room
  socket.join(`user:${socket.user.id}`);

  socket.on('disconnect', () => {
    console.log(`User disconnected: ${socket.user.username}`);
  });
});

server.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
  console.log('WebSocket server ready');
});
