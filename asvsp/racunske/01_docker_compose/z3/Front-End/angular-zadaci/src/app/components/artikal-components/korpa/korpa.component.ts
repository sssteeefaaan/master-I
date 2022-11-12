import { Component, OnInit } from '@angular/core';
import { Artikal } from '../../../models/Artikal';

import { KorpaService } from '../../../services/korpa.service';

@Component({
  selector: 'app-korpa',
  templateUrl: './korpa.component.html',
  styleUrls: ['./korpa.component.css']
})
export class KorpaComponent implements OnInit {
  artikli: Artikal[];

  constructor(private korpaService: KorpaService) { }

  ngOnInit() {
    this.korpaService.getArtikli().subscribe(artikli => this.artikli = artikli);
  }

}
