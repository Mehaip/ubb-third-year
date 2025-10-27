const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const jwt = require('jsonwebtoken');
const cors = require('cors');

const app = express();
const server = http.createServer(app);
const io = socketIo(server, { cors: { origin: '*' } });

app.use(cors());
app.use(express.json());

const SECRET = 'secret123';
const users = { user1: 'pass1', user2: 'pass2' };
let songs = [
  { id: 1, title: 'Song 1', artist: 'Artist 1', duration: '3:30', userId: 'user1' },
  { id: 2, title: 'Song 2', artist: 'Artist 2', duration: '4:00', userId: 'user1' },
  { id: 3, title: 'Song 3', artist: 'Artist 3', duration: '2:45', userId: 'user2' },
  { id: 4, title: 'Song 4', artist: 'Artist 4', duration: '3:15', userId: 'user1' },
  { id: 5, title: 'Song 5', artist: 'Artist 5', duration: '5:00', userId: 'user2' },
];

const auth = (req, res, next) => {
  const token = req.headers.authorization?.split(' ')[1];
  if (!token) return res.status(401).json({ error: 'No token' });
  try {
    req.user = jwt.verify(token, SECRET);
    next();
  } catch (e) {
    res.status(401).json({ error: 'Invalid token' });
  }
};

app.post('/login', (req, res) => {
  const { username, password } = req.body;
  if (users[username] === password) {
    const token = jwt.sign({ username }, SECRET);
    res.json({ token, username });
  } else {
    res.status(401).json({ error: 'Invalid credentials' });
  }
});

app.get('/songs', auth, (req, res) => {
  const page = parseInt(req.query.page) || 0;
  const limit = 2;
  const search = req.query.search || '';

  let filtered = songs.filter(s =>
    s.userId === req.user.username &&
    (s.title.toLowerCase().includes(search.toLowerCase()) ||
     s.artist.toLowerCase().includes(search.toLowerCase()))
  );

  const paginated = filtered.slice(page * limit, (page + 1) * limit);
  res.json({ songs: paginated, hasMore: (page + 1) * limit < filtered.length });
});

app.post('/songs', auth, (req, res) => {
  const song = { ...req.body, id: Date.now(), userId: req.user.username };
  songs.push(song);
  io.emit('song-added', { song, userId: req.user.username });
  res.json(song);
});

app.put('/songs/:id', auth, (req, res) => {
  const idx = songs.findIndex(s => s.id == req.params.id && s.userId === req.user.username);
  if (idx !== -1) {
    songs[idx] = { ...songs[idx], ...req.body };
    io.emit('song-updated', { song: songs[idx], userId: req.user.username });
    res.json(songs[idx]);
  } else {
    res.status(404).json({ error: 'Not found' });
  }
});

app.delete('/songs/:id', auth, (req, res) => {
  const idx = songs.findIndex(s => s.id == req.params.id && s.userId === req.user.username);
  if (idx !== -1) {
    songs.splice(idx, 1);
    io.emit('song-deleted', { id: req.params.id, userId: req.user.username });
    res.json({ success: true });
  } else {
    res.status(404).json({ error: 'Not found' });
  }
});

io.on('connection', (socket) => {
  console.log('Client connected');
  socket.on('disconnect', () => console.log('Client disconnected'));
});

server.listen(3001, () => console.log('Server on http://localhost:3001'));
