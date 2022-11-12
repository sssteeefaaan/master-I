import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ArtikalListComponent } from './artikal-list.component';

describe('ArtikalListComponent', () => {
  let component: ArtikalListComponent;
  let fixture: ComponentFixture<ArtikalListComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ArtikalListComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ArtikalListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
