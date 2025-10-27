import axios, { AxiosInstance } from 'axios';
import { MusicTrack, PaginatedResponse } from '../types';
import AuthService from './AuthService';

const API_URL = 'http://localhost:3001/api';

class ApiService {
  private axiosInstance: AxiosInstance;

  constructor() {
    this.axiosInstance = axios.create({
      baseURL: API_URL,
      timeout: 10000
    });

    // Add auth token to all requests
    this.axiosInstance.interceptors.request.use(
      (config) => {
        const token = AuthService.getAuthToken();
        if (token) {
          config.headers.Authorization = `Bearer ${token}`;
        }
        return config;
      },
      (error) => Promise.reject(error)
    );
  }

  async getTracks(page: number = 1, pageSize: number = 20, search?: string, genre?: string): Promise<PaginatedResponse<MusicTrack>> {
    const params: any = { page, pageSize };
    if (search) params.search = search;
    if (genre) params.genre = genre;

    const response = await this.axiosInstance.get<PaginatedResponse<MusicTrack>>('/tracks', { params });
    return response.data;
  }

  async getTrack(id: string): Promise<MusicTrack> {
    const response = await this.axiosInstance.get<MusicTrack>(`/tracks/${id}`);
    return response.data;
  }

  async createTrack(track: Omit<MusicTrack, 'id' | 'userId' | 'createdAt' | 'updatedAt'>): Promise<MusicTrack> {
    const response = await this.axiosInstance.post<MusicTrack>('/tracks', track);
    return response.data;
  }

  async updateTrack(id: string, track: Partial<MusicTrack>): Promise<MusicTrack> {
    const response = await this.axiosInstance.put<MusicTrack>(`/tracks/${id}`, track);
    return response.data;
  }

  async deleteTrack(id: string): Promise<void> {
    await this.axiosInstance.delete(`/tracks/${id}`);
  }
}

export default new ApiService();
