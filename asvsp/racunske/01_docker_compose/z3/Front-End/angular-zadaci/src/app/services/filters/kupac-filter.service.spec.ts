import { TestBed, inject } from '@angular/core/testing';

import { KupacFilterService } from './kupac-filter.service';

describe('KupacFilterService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [KupacFilterService]
    });
  });

  it('should be created', inject([KupacFilterService], (service: KupacFilterService) => {
    expect(service).toBeTruthy();
  }));
});
