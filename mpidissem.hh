
#ifndef MpiDissem_HH_
#define MpiDissem_HH_

#include <mpi.h>
#include "cx/synhax.hh"
#include "cx/table.hh"

class MpiDissem
{
private:
  bool done;
  bool term;
  uint degree;
  int value;
  int tag;
  MPI_Comm comm;
  Cx::Table<uint> hood;
  Cx::Table< uint > paysizes;
  Cx::Table< Cx::Table<uint> > payloads;
  Cx::Table<MPI_Request> requests;
  Cx::Table<MPI_Status > statuses;
  Cx::Table<Bool> done_flags;
  Cx::Table< Cx::Table<uint> > next_o_payloads;
  Cx::Table< int > indices;
public:
  uint PcIdx;
public:

  MpiDissem(uint _PcIdx, uint NPcs, int _tag, MPI_Comm _comm);

  uint sz() const { return this->degree; }

  int x_hood(uint i) { return hood[i]; }
  int o_hood(uint i) { return hood[this->sz() + i]; }
  uint* x_paysize(uint i) { return &paysizes[2*i]; }
  uint* o_paysize(uint i) { return &paysizes[2*(this->sz() + i)]; }
  Cx::Table<uint>& x_payload(uint i) { return payloads[i]; }
  Cx::Table<uint>& o_payload(uint i) { return payloads[this->sz() + i]; }
  MPI_Request* x_request(uint i) { return &requests[i]; }
  MPI_Request* o_request(uint i) { return &requests[this->sz() + i]; }
  MPI_Request* x_requests() { return this->x_request(0); }
  MPI_Request* o_requests() { return this->o_request(0); }
  MPI_Status* x_status(uint i) { return &statuses[i]; }
  MPI_Status* o_status(uint i) { return &statuses[this->sz() + i]; }
  MPI_Status* x_statuses() { return this->x_status(0); }
  MPI_Status* o_statuses() { return this->o_status(0); }
private:
  Bool& x_done_flag(uint i) { return done_flags[i]; }
  Bool& o_done_flag(uint i) { return done_flags[this->sz() + i]; }

private:
  void handle_recv(uint i);
  void handle_send(uint i);

public:
  bool xtestlite(Cx::Table<uint>& ret);
  bool xtest(Cx::Table<uint>& ret);
  bool xwait(Cx::Table<uint>& ret);
  void maysend();

  MpiDissem& operator<<(uint x) {
    for (uint i = 0; i < this->sz(); ++i) {
      this->next_o_payloads[i].push(x);
    }
    return *this;
  }

  void terminate();
  void done_fo();
  bool done_ck();
};

#endif
