import { TestBed, inject } from '@angular/core/testing';

import { TipArtiklaService } from './tip-artikla.service';

describe('TipArtiklaService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [TipArtiklaService]
    });
  });

  it('should be created', inject([TipArtiklaService], (service: TipArtiklaService) => {
    expect(service).toBeTruthy();
  }));
});
