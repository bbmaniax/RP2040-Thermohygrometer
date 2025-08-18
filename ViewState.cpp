// ViewState - Manages the current view mode and display orientation.

#include <Arduino.h>

#include "ViewState.h"

ViewState::ViewState() : viewMode(VIEW_MODE_ALL), displayFlipped(false) {}

void ViewState::begin() {
  viewMode = VIEW_MODE_ALL;
  displayFlipped = false;
}

ViewState::ViewMode ViewState::getViewMode() const {
  return viewMode;
}

void ViewState::switchToNextViewMode() {
  viewMode = (ViewMode)(((int)viewMode + 1) % (int)VIEW_MODE_COUNT);
}

bool ViewState::isDisplayFlipped() const {
  return displayFlipped;
}

void ViewState::flipDisplay() {
  displayFlipped = !displayFlipped;
}
