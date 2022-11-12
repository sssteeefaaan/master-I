import { TestBed, inject } from '@angular/core/testing';

import { KorpaService } from './korpa.service';

describe('KorpaService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [KorpaService]
    });
  });

  it('should be created', inject([KorpaService], (service: KorpaService) => {
    expect(service).toBeTruthy();
  }));
});
