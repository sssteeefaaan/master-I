import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { MessageService } from './message.service';
import { AuthService } from './auth.service';
import { environment } from '../../environments/environment';
import { Observable, of } from 'rxjs';
import { Artikal } from '../models/Artikal';
import { tap, catchError } from 'rxjs/operators';

@Injectable({
  providedIn: 'root'
})
export class KorpaService {

  private korpaUrl = environment.apiBaseUrl + '/korpa/';

  constructor(private httpClient: HttpClient,
              private messageService: MessageService,
              private authService: AuthService) { }

  getArtikli(): Observable<Artikal[]> {
    return this.httpClient
      .get<Artikal[]>(this.korpaUrl, {headers: this.authService.getHeaders()})
      .pipe(
        tap(_ => this.log(`Učitani artikli iz korpe`)),
        catchError(this.handleError<Artikal[]>('getArtikli', [])));
  }

  dodajUKorpu(artikal: Artikal): Observable<Artikal> {
    return this.httpClient
      .post<any>(this.korpaUrl, artikal, {headers: this.authService.getHeaders()})
      .pipe(
        tap(a => this.log(`Artikal sa id=${a.id} dodat u korpu`)),
        catchError(this.handleError<Artikal>('dodajUKorpu')));
  }

  private log(message: string) {
    this.messageService.add('KorpaService: ' + message);
  }

  private handleError<T> (operation = 'operation', result?: T) {
    return (error: any): Observable<T> => {
      console.error(error);
      this.log(`${operation} failed: ${error.message}`);
      return of(result as T);
    };
  }
}
