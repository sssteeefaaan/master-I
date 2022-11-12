import { Component, OnInit, AfterViewInit } from '@angular/core';
import { Observable, Subject } from 'rxjs';
import { debounceTime, distinctUntilChanged, switchMap, tap } from 'rxjs/operators';

import { Artikal } from '../../../models/Artikal';
import { ArtikalService } from '../../../services/artikal.service';
import { AuthService } from '../../../services/auth.service';

@Component({
  selector: 'app-artikal-list',
  templateUrl: './artikal-list.component.html',
  styleUrls: ['./artikal-list.component.css']
})
export class ArtikalListComponent implements OnInit, AfterViewInit {

  artikli$: Observable<Artikal[]>;
  private searchTerm = new Subject<string>();

  constructor(private artikalService: ArtikalService, private authService: AuthService) { }

  search(term: string) {
    this.searchTerm.next(term);
  }

  ngOnInit() {
    this.artikli$ = this.searchTerm.pipe(
      debounceTime(500),
      distinctUntilChanged(),
      switchMap((term: string) => this.artikalService.searchArtikli(term))
    );
  }

  ngAfterViewInit(): void {
    this.search('');
  }

}
