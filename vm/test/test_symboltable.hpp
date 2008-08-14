#include "objects.hpp"
#include "vm.hpp"
#include "objectmemory.hpp"
#include "symboltable.hpp"

#include <cxxtest/TestSuite.h>

using namespace rubinius;

class TestSymbolTable : public CxxTest::TestSuite {
  public:

  VM *state;
  SymbolTable* symbols;

  void setUp() {
    state = new VM(1024);
    symbols = new SymbolTable();
  }

  void tearDown() {
    delete state;
  }

  void test_lookup() {
    OBJECT sym = symbols->lookup(state, "unique");
    TS_ASSERT(sym->symbol_p());

    OBJECT sym2 = symbols->lookup(state, "unique");
    TS_ASSERT_EQUALS(sym, sym2);
  }

  void test_lookup_colliding_hash() {
    OBJECT sym, sym2;

    sym  = symbols->lookup(state, "__uint_fast64_t");
    sym2 = symbols->lookup(state, "TkIF_MOD");

    TS_ASSERT(sym != sym2);
  }

  void test_lookup_colliding_hash2() {
    String* a = String::create(state, "__uint_fast64_t");
    String* b = String::create(state, "TkIF_MOD");

    TS_ASSERT_EQUALS(a->hash_string(state), b->hash_string(state));

    OBJECT sym, sym2;

    sym  = symbols->lookup(state, a);
    sym2 = symbols->lookup(state, b);

    TS_ASSERT(sym != sym2);
  }

  void test_symbol_to_string() {
    Symbol* sym = symbols->lookup(state, "circle");
    String* str = symbols->symbol_to_string(state, sym);

    TS_ASSERT(!strncmp("circle", str->byte_address(state), 6));
  }

  void test_size() {
    symbols->lookup(state, "uno");
    symbols->lookup(state, "dos");
    symbols->lookup(state, "tres");

    TS_ASSERT_EQUALS(symbols->size(), 3)
  }

  void test_many_symbols() {
    size_t size = symbols->size();

    for(size_t i = 0; i < 100; i++) {
      std::stringstream stream;
      stream << "sym" << i;
      symbols->lookup(state, stream.str().c_str());
    }

    TS_ASSERT(symbols->size() > size);
  }
};
