import axios from 'axios';
import { AuthResponse, LoginCredentials, User } from '../types';
import StorageService from './StorageService';

const API_URL = 'http://localhost:3001/api';

class AuthService {
  private currentUser: User | null = null;

  async init(): Promise<void> {
    const token = await StorageService.getAuthToken();
    const user = await StorageService.getUser();

    if (token && user) {
      this.currentUser = { ...user, token };
    }
  }

  async login(credentials: LoginCredentials): Promise<User> {
    try {
      const response = await axios.post<AuthResponse>(`${API_URL}/auth/login`, credentials);
      const { token, user } = response.data;

      const currentUser: User = {
        id: user.id,
        username: user.username,
        token
      };

      this.currentUser = currentUser;
      await StorageService.saveAuthToken(token);
      await StorageService.saveUser(currentUser);

      return currentUser;
    } catch (error) {
      console.error('Login failed:', error);
      throw new Error('Invalid credentials');
    }
  }

  async logout(): Promise<void> {
    this.currentUser = null;
    await StorageService.clearAuth();
  }

  isAuthenticated(): boolean {
    return this.currentUser !== null;
  }

  getCurrentUser(): User | null {
    return this.currentUser;
  }

  getAuthToken(): string | null {
    return this.currentUser?.token || null;
  }
}

export default new AuthService();
