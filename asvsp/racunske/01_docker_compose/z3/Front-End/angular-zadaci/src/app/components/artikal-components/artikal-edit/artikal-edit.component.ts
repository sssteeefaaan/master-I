import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { Location } from '@angular/common';

import { Artikal } from '../../../models/Artikal';
import { ArtikalService } from '../../../services/artikal.service';
import { TipArtiklaService } from '../../../services/tip-artikla.service';
import { TipArtikla } from '../../../models/TipArtikla';

@Component({
  selector: 'app-artikal-edit',
  templateUrl: './artikal-edit.component.html',
  styleUrls: ['./artikal-edit.component.css']
})
export class ArtikalEditComponent implements OnInit {
  artikal: Artikal;
  tipovi: TipArtikla[] = [];

  constructor(private route: ActivatedRoute,
              private router: Router,
              private location: Location,
              private artikalService: ArtikalService,
              private tipArtiklaService: TipArtiklaService) {
    this.artikal = new Artikal();
  }

  ngOnInit() {
    const id = +this.route.snapshot.paramMap.get('id');
    this.artikalService.getArtikal(id).subscribe(a => this.artikal = a);
    this.getTipovi();
  }

  getTipovi() {
    this.tipArtiklaService.getTipovi().subscribe(t => this.tipovi = t);
  }

  editArtikal() {
    this.artikalService.updateArtikal(this.artikal)
    .subscribe((artikal: Artikal) =>  {
      alert('Artikal ' + artikal.naziv + ' je uspešno izmenjen!');
      this.router.navigate(['/artikli']);
    });
  }

  goBack() {
    this.location.back();
  }

}
