import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ArtikalAddComponent } from './artikal-add.component';

describe('ArtikalAddComponent', () => {
  let component: ArtikalAddComponent;
  let fixture: ComponentFixture<ArtikalAddComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ArtikalAddComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ArtikalAddComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
