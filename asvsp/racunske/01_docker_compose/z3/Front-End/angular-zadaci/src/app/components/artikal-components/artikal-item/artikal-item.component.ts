import { Component, OnInit, Input } from '@angular/core';
import { Artikal } from '../../../models/Artikal';
import { AuthService } from '../../../services/auth.service';
import { KorpaService } from '../../../services/korpa.service';
import { ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-artikal-item',
  templateUrl: './artikal-item.component.html',
  styleUrls: ['./artikal-item.component.css']
})
export class ArtikalItemComponent implements OnInit {
  @Input() artikal: Artikal;
  activeRoute: string;

  constructor(private authService: AuthService,
              private korpaService: KorpaService,
              private activatedRoute: ActivatedRoute) { }

  ngOnInit() {
    this.activatedRoute.url.subscribe(x =>  this.activeRoute = x[0].path);
  }

  dodajUKorpu() {
    this.korpaService.dodajUKorpu(this.artikal).subscribe(_ => alert(`Artikal dodat u korpu!`));
  }
}
