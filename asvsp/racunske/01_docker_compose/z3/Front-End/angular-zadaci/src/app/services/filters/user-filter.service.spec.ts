import { TestBed, inject } from '@angular/core/testing';

import { UserFilterService } from './user-filter.service';

describe('UserFilterService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [UserFilterService]
    });
  });

  it('should be created', inject([UserFilterService], (service: UserFilterService) => {
    expect(service).toBeTruthy();
  }));
});
