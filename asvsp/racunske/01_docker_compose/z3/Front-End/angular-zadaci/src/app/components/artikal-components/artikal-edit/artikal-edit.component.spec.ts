import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ArtikalEditComponent } from './artikal-edit.component';

describe('ArtikalEditComponent', () => {
  let component: ArtikalEditComponent;
  let fixture: ComponentFixture<ArtikalEditComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ArtikalEditComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ArtikalEditComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
