import React, { useState } from 'react';
import { IonPage, IonHeader, IonToolbar, IonTitle, IonContent, IonInput, IonButton, IonItem, IonLabel, IonText } from '@ionic/react';
import { useHistory } from 'react-router-dom';
import AuthService from '../services/AuthService';
import WebSocketService from '../services/WebSocketService';
import { useApp } from '../context/AppContext';

const LoginPage: React.FC = () => {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const history = useHistory();
  const { setUser } = useApp();

  const handleLogin = async () => {
    setError('');
    setLoading(true);

    try {
      const user = await AuthService.login({ username, password });
      setUser(user);
      WebSocketService.connect();
      history.replace('/tracks');
    } catch (err) {
      setError('Invalid username or password');
    } finally {
      setLoading(false);
    }
  };

  return (
    <IonPage>
      <IonHeader>
        <IonToolbar>
          <IonTitle>Music Player - Login</IonTitle>
        </IonToolbar>
      </IonHeader>
      <IonContent className="ion-padding">
        <div style={{ maxWidth: '400px', margin: '50px auto' }}>
          <h2>Login</h2>

          <IonItem>
            <IonLabel position="floating">Username</IonLabel>
            <IonInput
              value={username}
              onIonInput={(e) => setUsername(e.detail.value!)}
              type="text"
            />
          </IonItem>

          <IonItem>
            <IonLabel position="floating">Password</IonLabel>
            <IonInput
              value={password}
              onIonInput={(e) => setPassword(e.detail.value!)}
              type="password"
            />
          </IonItem>

          {error && (
            <IonText color="danger">
              <p>{error}</p>
            </IonText>
          )}

          <IonButton
            expand="block"
            onClick={handleLogin}
            disabled={loading || !username || !password}
            style={{ marginTop: '20px' }}
          >
            {loading ? 'Logging in...' : 'Login'}
          </IonButton>

          <div style={{ marginTop: '30px', fontSize: '14px', color: '#666' }}>
            <p>Test credentials:</p>
            <p>Username: user1, Password: password1</p>
            <p>Username: user2, Password: password2</p>
          </div>
        </div>
      </IonContent>
    </IonPage>
  );
};

export default LoginPage;
