import { TestBed, inject } from '@angular/core/testing';

import { ArtikalService } from './artikal.service';

describe('ArtikalService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [ArtikalService]
    });
  });

  it('should be created', inject([ArtikalService], (service: ArtikalService) => {
    expect(service).toBeTruthy();
  }));
});
