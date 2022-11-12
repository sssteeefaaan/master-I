import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ArtikalItemComponent } from './artikal-item.component';

describe('ArtikalItemComponentComponent', () => {
  let component: ArtikalItemComponent;
  let fixture: ComponentFixture<ArtikalItemComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ArtikalItemComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ArtikalItemComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
