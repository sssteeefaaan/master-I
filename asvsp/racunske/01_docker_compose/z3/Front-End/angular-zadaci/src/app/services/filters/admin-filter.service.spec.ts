import { TestBed, inject } from '@angular/core/testing';

import { AdminFilterService } from './admin-filter.service';

describe('AdminFilterService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [AdminFilterService]
    });
  });

  it('should be created', inject([AdminFilterService], (service: AdminFilterService) => {
    expect(service).toBeTruthy();
  }));
});
