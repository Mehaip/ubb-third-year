import React from 'react';
import { Redirect, Route } from 'react-router-dom';
import { IonApp, IonRouterOutlet, setupIonicReact } from '@ionic/react';
import { IonReactRouter } from '@ionic/react-router';
import { AppProvider, useApp } from './context/AppContext';
import LoginPage from './pages/LoginPage';
import MusicListPage from './pages/MusicListPage';
import MusicDetailPage from './pages/MusicDetailPage';

import '@ionic/react/css/core.css';
import '@ionic/react/css/normalize.css';
import '@ionic/react/css/structure.css';
import '@ionic/react/css/typography.css';
import '@ionic/react/css/padding.css';
import '@ionic/react/css/float-elements.css';
import '@ionic/react/css/text-alignment.css';
import '@ionic/react/css/text-transformation.css';
import '@ionic/react/css/flex-utils.css';
import '@ionic/react/css/display.css';

setupIonicReact();

const PrivateRoute: React.FC<{ component: React.FC; path: string; exact?: boolean }> = ({
  component: Component,
  path,
  exact = false
}) => {
  const { isAuthenticated } = useApp();

  return (
    <Route
      path={path}
      exact={exact}
      render={(props) =>
        isAuthenticated ? <Component {...props} /> : <Redirect to="/login" />
      }
    />
  );
};

const AppRoutes: React.FC = () => {
  const { isAuthenticated } = useApp();

  return (
    <IonReactRouter>
      <IonRouterOutlet>
        <Route exact path="/login" component={LoginPage} />
        <PrivateRoute exact path="/tracks" component={MusicListPage} />
        <PrivateRoute exact path="/tracks/:id" component={MusicDetailPage} />
        <Route exact path="/">
          {isAuthenticated ? <Redirect to="/tracks" /> : <Redirect to="/login" />}
        </Route>
      </IonRouterOutlet>
    </IonReactRouter>
  );
};

const App: React.FC = () => {
  return (
    <IonApp>
      <AppProvider>
        <AppRoutes />
      </AppProvider>
    </IonApp>
  );
};

export default App;
