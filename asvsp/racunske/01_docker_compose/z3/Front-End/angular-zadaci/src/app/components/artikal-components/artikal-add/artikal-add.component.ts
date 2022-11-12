import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { Location } from '@angular/common';

import { Artikal } from '../../../models/Artikal';
import { TipArtikla } from '../../../models/TipArtikla';
import { ArtikalService } from '../../../services/artikal.service';
import { TipArtiklaService } from '../../../services/tip-artikla.service';


@Component({
  selector: 'app-artikal-add',
  templateUrl: './artikal-add.component.html',
  styleUrls: ['./artikal-add.component.css']
})
export class ArtikalAddComponent implements OnInit {
  artikal: Artikal;
  tipovi: TipArtikla[] = [];

  constructor(private router: Router,
    private location: Location,
    private artikalService: ArtikalService,
    private tipArtiklaService: TipArtiklaService) {
    this.artikal = new Artikal();
  }

  ngOnInit() {
    this.getTipovi();
  }

  getTipovi() {
    this.tipArtiklaService.getTipovi().subscribe(
      t => { this.tipovi = t; this.artikal.tipArtikla = this.tipovi[0].naziv; }
    );
  }

  addArtikal(sifra: string, naziv: string, cena: number, opis: string) {
    this.artikal.sifra = sifra;
    this.artikal.naziv = naziv;
    this.artikal.cena = cena;
    this.artikal.opis = opis;
    this.artikal.slikaUrl = '';

    this.artikalService.addArtikal(this.artikal)
      .subscribe((artikal: Artikal) => {
        alert('Artikal ' + artikal.naziv + ' je uspešno dodat!');
        this.router.navigate(['/artikli']);
      });
  }

  goBack() {
    this.location.back();
  }

}
