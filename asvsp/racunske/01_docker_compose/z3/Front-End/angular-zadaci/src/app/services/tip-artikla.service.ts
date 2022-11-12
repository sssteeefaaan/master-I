import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { catchError, map, tap } from 'rxjs/operators';

import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';

import { MessageService } from './message.service';
import { TipArtikla } from './../models/TipArtikla';
import { environment } from '../../environments/environment';
import { AuthService } from './auth.service';

@Injectable({
  providedIn: 'root'
})
export class TipArtiklaService {
  private tipoviUrl = environment.apiBaseUrl + '/tipArtikla/';

  constructor(private httpClient: HttpClient,
              private messageService: MessageService,
              private authService: AuthService) { }

  getTipovi(): Observable<TipArtikla[]> {
    return this.httpClient
      .get<TipArtikla[]>(this.tipoviUrl, {headers: this.authService.getHeaders()})
      .pipe(
        tap(_ => this.log(`Učitani tipova artikala`)),
        catchError(this.handleError<TipArtikla[]>('getTipovi', [])));
  }

  private log(message: string) {
    this.messageService.add('TipArtiklaService: ' + message);
  }

  private handleError<T> (operation = 'operation', result?: T) {
    return (error: any): Observable<T> => {
      console.error(error);
      this.log(`${operation} failed: ${error.message}`);
      return of(result as T);
    };
  }
}
