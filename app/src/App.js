import React, { useState, useEffect, useRef } from 'react';
import axios from 'axios';
import io from 'socket.io-client';

const API = 'http://localhost:3001';

export default function App() {
  const [token, setToken] = useState(localStorage.getItem('token'));
  const [username, setUsername] = useState(localStorage.getItem('username'));
  const [songs, setSongs] = useState([]);
  const [selected, setSelected] = useState(null);
  const [page, setPage] = useState(0);
  const [hasMore, setHasMore] = useState(true);
  const [search, setSearch] = useState('');
  const [online, setOnline] = useState(navigator.onLine);
  const [pending, setPending] = useState(JSON.parse(localStorage.getItem('pending') || '[]'));
  const [form, setForm] = useState({ title: '', artist: '', duration: '' });
  const socket = useRef(null);

  useEffect(() => {
    const handleOnline = () => {
      setOnline(true);
      syncPending();
    };
    const handleOffline = () => setOnline(false);
    window.addEventListener('online', handleOnline);
    window.addEventListener('offline', handleOffline);
    return () => {
      window.removeEventListener('online', handleOnline);
      window.removeEventListener('offline', handleOffline);
    };
  }, []);

  useEffect(() => {
    if (token) {
      loadSongs();
      socket.current = io(API);
      socket.current.on('song-added', (data) => {
        if (data.userId === username) setSongs(s => [...s, data.song]);
      });
      socket.current.on('song-updated', (data) => {
        if (data.userId === username) setSongs(s => s.map(x => x.id === data.song.id ? data.song : x));
      });
      socket.current.on('song-deleted', (data) => {
        if (data.userId === username) setSongs(s => s.filter(x => x.id !== data.id));
      });
      return () => socket.current?.disconnect();
    }
  }, [token, search]);

  const loadSongs = async () => {
    try {
      const res = await axios.get(`${API}/songs?page=${page}&search=${search}`, {
        headers: { Authorization: `Bearer ${token}` }
      });
      setSongs(prev => page === 0 ? res.data.songs : [...prev, ...res.data.songs]);
      setHasMore(res.data.hasMore);
      localStorage.setItem('songs', JSON.stringify(res.data.songs));
    } catch (e) {
      const cached = JSON.parse(localStorage.getItem('songs') || '[]');
      setSongs(cached);
    }
  };

  const syncPending = async () => {
    if (!token || pending.length === 0) return;
    const newPending = [];
    for (const p of pending) {
      try {
        if (p.action === 'create') {
          await axios.post(`${API}/songs`, p.data, { headers: { Authorization: `Bearer ${token}` } });
        } else if (p.action === 'update') {
          await axios.put(`${API}/songs/${p.id}`, p.data, { headers: { Authorization: `Bearer ${token}` } });
        } else if (p.action === 'delete') {
          await axios.delete(`${API}/songs/${p.id}`, { headers: { Authorization: `Bearer ${token}` } });
        }
      } catch (e) {
        newPending.push(p);
      }
    }
    setPending(newPending);
    localStorage.setItem('pending', JSON.stringify(newPending));
    loadSongs();
  };

  const login = async (e) => {
    e.preventDefault();
    try {
      const res = await axios.post(`${API}/login`, { username: e.target.username.value, password: e.target.password.value });
      setToken(res.data.token);
      setUsername(res.data.username);
      localStorage.setItem('token', res.data.token);
      localStorage.setItem('username', res.data.username);
    } catch (e) {
      alert('Login failed');
    }
  };

  const logout = () => {
    setToken(null);
    setUsername(null);
    localStorage.removeItem('token');
    localStorage.removeItem('username');
  };

  const createSong = async () => {
    if (online) {
      try {
        await axios.post(`${API}/songs`, form, { headers: { Authorization: `Bearer ${token}` } });
        setForm({ title: '', artist: '', duration: '' });
        loadSongs();
      } catch (e) {
        addPending('create', form);
      }
    } else {
      addPending('create', form);
    }
  };

  const updateSong = async () => {
    if (online) {
      try {
        await axios.put(`${API}/songs/${selected.id}`, form, { headers: { Authorization: `Bearer ${token}` } });
        setSelected(null);
        setForm({ title: '', artist: '', duration: '' });
        loadSongs();
      } catch (e) {
        addPending('update', form, selected.id);
      }
    } else {
      addPending('update', form, selected.id);
    }
  };

  const deleteSong = async (id) => {
    if (online) {
      try {
        await axios.delete(`${API}/songs/${id}`, { headers: { Authorization: `Bearer ${token}` } });
        loadSongs();
      } catch (e) {
        addPending('delete', null, id);
      }
    } else {
      addPending('delete', null, id);
    }
  };

  const addPending = (action, data, id = null) => {
    const p = [...pending, { action, data, id }];
    setPending(p);
    localStorage.setItem('pending', JSON.stringify(p));
    alert('Saved offline, will sync when online');
  };

  useEffect(() => {
    if (page > 0) loadSongs();
  }, [page]);

  if (!token) {
    return (
      <div style={{ padding: 20 }}>
        <h2>Login</h2>
        <form onSubmit={login}>
          <input name="username" placeholder="Username" required />
          <input name="password" type="password" placeholder="Password" required />
          <button type="submit">Login</button>
        </form>
        <p>Try: user1/pass1 or user2/pass2</p>
      </div>
    );
  }

  return (
    <div style={{ padding: 20 }}>
      <div style={{ background: online ? 'green' : 'red', color: 'white', padding: 10 }}>
        {online ? 'Online' : 'Offline'} | User: {username} <button onClick={logout}>Logout</button>
      </div>
      {pending.length > 0 && <div style={{ background: 'orange', padding: 10 }}>
        {pending.length} items pending sync
      </div>}

      <h2>Songs</h2>
      <input placeholder="Search" value={search} onChange={(e) => { setSearch(e.target.value); setPage(0); }} />

      <div style={{ display: 'flex', gap: 20, marginTop: 20 }}>
        <div style={{ flex: 1 }}>
          <h3>List</h3>
          {songs.map(s => (
            <div key={s.id} onClick={() => setSelected(s)} style={{ padding: 10, cursor: 'pointer', background: selected?.id === s.id ? '#eee' : 'white', border: '1px solid #ccc' }}>
              {s.title} - {s.artist}
            </div>
          ))}
          {hasMore && <button onClick={() => setPage(p => p + 1)}>Load More</button>}
        </div>

        <div style={{ flex: 1 }}>
          <h3>{selected ? 'Edit Song' : 'Add Song'}</h3>
          <input placeholder="Title" value={form.title} onChange={(e) => setForm({ ...form, title: e.target.value })} />
          <input placeholder="Artist" value={form.artist} onChange={(e) => setForm({ ...form, artist: e.target.value })} />
          <input placeholder="Duration" value={form.duration} onChange={(e) => setForm({ ...form, duration: e.target.value })} />
          {selected ? (
            <>
              <button onClick={updateSong}>Update</button>
              <button onClick={() => deleteSong(selected.id)}>Delete</button>
              <button onClick={() => { setSelected(null); setForm({ title: '', artist: '', duration: '' }); }}>Cancel</button>
            </>
          ) : (
            <button onClick={createSong}>Create</button>
          )}
          {selected && (
            <div style={{ marginTop: 20, padding: 10, border: '1px solid #ccc' }}>
              <h4>Details</h4>
              <p>Title: {selected.title}</p>
              <p>Artist: {selected.artist}</p>
              <p>Duration: {selected.duration}</p>
            </div>
          )}
        </div>
      </div>
    </div>
  );
}
