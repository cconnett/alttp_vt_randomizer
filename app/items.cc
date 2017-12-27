#include "items.h"
#include "arraylength.h"

int INITIAL_UNPLACED[(int)Item::NUM_ITEMS];

void count_items() {
  INITIAL_UNPLACED[(int)Item::DefeatAgahnim] = 1;
  INITIAL_UNPLACED[(int)Item::DefeatAgahnim2] = 1;
  INITIAL_UNPLACED[(int)Item::DefeatGanon] = 1;
  for (unsigned int i = 0; i < ARRAY_LENGTH(PRIZES); i++) {
    INITIAL_UNPLACED[(int)PRIZES[i]]++;
  }
  for (unsigned int i = 0; i < ARRAY_LENGTH(DUNGEON_ITEMS); i++) {
    for (const Item *j = DUNGEON_ITEMS[i]; *j != Item::INVALID; j++) {
      INITIAL_UNPLACED[(int)*j]++;
    }
  }
  for (unsigned int i = 0; i < ARRAY_LENGTH(ADVANCEMENT_ITEMS); i++) {
    INITIAL_UNPLACED[(int)ADVANCEMENT_ITEMS[i]]++;
  }
  for (unsigned int i = 0; i < ARRAY_LENGTH(NICE_ITEMS); i++) {
    INITIAL_UNPLACED[(int)NICE_ITEMS[i]]++;
  }
  for (unsigned int i = 0; i < ARRAY_LENGTH(TRASH_ITEMS); i++) {
    INITIAL_UNPLACED[(int)TRASH_ITEMS[i]]++;
  }
}
