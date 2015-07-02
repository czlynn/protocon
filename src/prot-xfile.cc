

#include "prot-xfile.hh"
#include "cx/bittable.hh"

  bool
ProtoconFile::update_allgood(bool good)
{
  if (!allgood)  return false;
  if (good)  return true;
  allgood = false;
  DBog1( "Something terribly wrong at/after line:%u", this->text_nlines+1 );
  return false;
}

  void
ProtoconFile::bad_parse(const char* text, const char* reason)
{
  Cx::OFile ofile(stderr_OFile ());
  ofile << "Error at" << (text ? "" : "/after") << " line " << (this->text_nlines+1);
  if (text) {
     ofile << " in text: " << text;
  }
  if (reason) {
    ofile << "\nReason for error: " << reason;
  }
  ofile << ofile.endl();
  this->allgood = false;
}

  bool
ProtoconFile::add_variables(Sesp vbl_name_sp, Sesp vbl_nmembs_sp, Sesp vbl_dom_sp,
                            Xn::Vbl::ShadowPuppetRole role)
{
  if (!allgood)  return false;
  DeclLegit( good );
  const char* name = 0;
  uint nmembs = 0;
  uint domsz = 0;

  DoLegitLineP(name, "")
    ccstr_of_Sesp (vbl_name_sp);
  DoLegitLine("Domain of variable index strange.")
    eq_cstr ("NatDom", ccstr_of_Sesp (car_of_Sesp (vbl_nmembs_sp)));
  DoLegitLine("Domain of variable is strange.")
    eq_cstr ("NatDom", ccstr_of_Sesp (car_of_Sesp (vbl_dom_sp)));

  DoLegitLine( "" )
    eval_gtz (&nmembs, cadr_of_Sesp (vbl_nmembs_sp));

  DoLegitLine( "" )
    eval_gtz (&domsz, cadr_of_Sesp (vbl_dom_sp));

  DoLegit(0)
  {
    Xn::VblSymm* symm = sys->topology.add_variables(name, nmembs, domsz, role);
    vbl_map[name] = symm;
    symm->spec->domsz_expression = "";
    string_expression (symm->spec->domsz_expression, cadr_of_Sesp (vbl_dom_sp));
    symm->spec->nmembs_expression = "";
    string_expression (symm->spec->nmembs_expression, cadr_of_Sesp (vbl_nmembs_sp));
  }
  return update_allgood (good);
}

  bool
ProtoconFile::add_processes(Sesp pc_name, Sesp idx_name, Sesp npcs)
{
  if (!allgood)  return false;
  Claim2( index_map.sz() ,==, 0 );
  DeclLegit( good );
  const char* name_a;
  const char* name_b;

  DoLegitLineP( name_a, 0 )
    ccstr_of_Sesp (pc_name);
  DoLegitLineP( name_b, 0 )
    ccstr_of_Sesp (idx_name);

  uint domsz = 0;
  DoLegitLine( "" )
    eval_nat (&domsz, cadr_of_Sesp (npcs));

  DoLegit( 0 ) {
    this->pc_symm =
      sys->topology.add_processes(name_a, name_b, (uint) domsz);
    this->pc_symm_spec = pc_symm->spec;
    pc_symm_spec->nmembs_expression = "";
    string_expression (pc_symm_spec->nmembs_expression, cadr_of_Sesp (npcs));
  }
  return update_allgood (good);
}

  bool
ProtoconFile::add_constant(Sesp name_sp, Sesp val_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );

  const char* name;
  DoLegitLineP( name, "" )
    ccstr_of_Sesp (name_sp);
  Xn::NatMap val(1);
  DoLegitLine("evaluating int")
    eval_int (&val.membs[0], val_sp);
  DoLegitLine( "finding expression" )
    string_expression (val.expression, val_sp);
  DoLegit(0)
    if (!spec->constant_map.key_ck(name))
      spec->constant_map.add(name, val);

  return update_allgood (good);
}

  bool
ProtoconFile::add_let(Sesp name_sp, Sesp val_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );
  const char* name;
  DoLegitLineP( name, "" )
    ccstr_of_Sesp (name_sp);

  const Cx::String& idx_name = pc_symm_spec->idx_name;
  Xn::NatMap let_vals( pc_symm->membs.sz() );
  for (uint i = 0; good && i < pc_symm->membs.sz(); ++i) {
    index_map[idx_name] = i;
    DoLegitLine( "" )
      eval_int (&let_vals.membs[i], val_sp);
  }
  index_map.erase(idx_name);
  DoLegitLine( "finding expression" )
    string_expression (let_vals.expression, val_sp);
  DoLegit(0)
    pc_symm_spec->let_map.add(name, let_vals);
  return update_allgood (good);
}

  bool
ProtoconFile::add_scope_let(Sesp name_sp, Sesp val_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );
  const char* name;
  DoLegitLineP( name, "" )
    ccstr_of_Sesp (name_sp);

  DoLegit(0)
    scope_let_map[name] = val_sp;

  return update_allgood (good);
}

  void
ProtoconFile::del_scope_let(Sesp name_sp)
{
  scope_let_map.erase(ccstr_of_Sesp(name_sp));
}

  bool
ProtoconFile::add_access(Sesp vbl_sp, Bit write, Bit random)
{
  if (!allgood)  return false;
  DeclLegit( good );
  const char* vbl_name = 0;
  Sesp vbl_idx_sp = 0;
  Xn::Net& topo = sys->topology;
  const Xn::VblSymm* vbl_symm = 0;

  // (aref vbl_name vbl_idx)
  DoLegitLineP( vbl_name, "" )
    ccstr_of_Sesp (cadr_of_Sesp (vbl_sp));

  DoLegitP( vbl_symm, "" )
  {
    const Xn::VblSymm** tmp = 0;
    tmp = vbl_map.lookup(vbl_name);
    if (tmp)  vbl_symm = *tmp;
  }

  DoLegit(0)
    vbl_idx_sp = caddr_of_Sesp (vbl_sp);

  DoLegitLine("")
    topo.pc_symms.sz() > 0;

  DoLegit("")
  {
    const Cx::String& idx_name = pc_symm_spec->idx_name;
    Xn::NatMap indices( pc_symm->membs.sz() );

    for (uint i = 0; i < pc_symm->membs.sz(); ++i) {
      index_map[idx_name] = i;
      DoLegitLine("")
        eval_int (&indices.membs[i], vbl_idx_sp);
    }
    index_map.erase(idx_name);

    DoLegitLine( "string_expression()" )
      string_expression(indices.expression, vbl_idx_sp);

    DoLegit(0) {
      if (write) {
        topo.add_write_access(+pc_symm, vbl_symm, indices);
        if (random) {
          pc_symm_spec->random_write_flags.top() = 1;
        }
      }
      else {
        topo.add_read_access(+pc_symm, vbl_symm, indices);
      }
    }
  }

  return update_allgood (good);
}

  bool
ProtoconFile::add_symmetric_links(Sesp let_names_sp, Sesp let_vals_list_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );
  pc_symm_spec->link_symmetries.push(Xn::LinkSymmetry(sz_of_Sesp (let_vals_list_sp)));
  Xn::LinkSymmetry& link_symmetry = pc_symm_spec->link_symmetries.top();
  if (sz_of_Sesp (let_names_sp) == 1) {
    link_symmetry.let_expression = ccstr_of_Sesp (car_of_Sesp (let_names_sp));
    Sesp let_vals_sp = let_vals_list_sp;
    while (!nil_ck_Sesp (let_vals_sp)) {
      link_symmetry.multiset_expression.push_delim("", ", ");
      Cx::String val_expression;
      string_expression(val_expression, caar_of_Sesp (let_vals_sp));
      link_symmetry.multiset_expression += val_expression;
      let_vals_sp = cdr_of_Sesp (let_vals_sp);
    }
    return update_allgood (good);
  }

  Sesp let_name_sp = let_names_sp;
  while (!nil_ck_Sesp (let_name_sp)) {
    link_symmetry.let_expression.push_delim("(", ", ");

    link_symmetry.let_expression += ccstr_of_Sesp (car_of_Sesp (let_name_sp));
    let_name_sp = cdr_of_Sesp (let_name_sp);
  }
  link_symmetry.let_expression += ")";

  Sesp let_vals_sp = let_vals_list_sp;
  while (!nil_ck_Sesp (let_vals_sp)) {
    link_symmetry.multiset_expression.push_delim("", ", ");

    Cx::String tuple_expression;
    Sesp let_val_sp = car_of_Sesp (let_vals_sp);
    while (!nil_ck_Sesp (let_val_sp)) {
      tuple_expression.push_delim("(", ", ");

      Cx::String val_expression;
      DoLegitLine( "" )
        string_expression(val_expression, car_of_Sesp (let_val_sp));
      if (!good)  break;
      tuple_expression += val_expression;
      let_val_sp = cdr_of_Sesp (let_val_sp);
    }
    tuple_expression += ")";

    link_symmetry.multiset_expression += tuple_expression;
    let_vals_sp = cdr_of_Sesp (let_vals_sp);
  }

  return update_allgood (good);
}

  bool
ProtoconFile::add_symmetric_access(Sesp let_names_sp, Sesp let_vals_list_sp,
                                   Sesp vbls_sp, Bit write, Bit random)
{
  if (!allgood)  return false;
  DeclLegit( good );
  Xn::LinkSymmetry& link_symmetry = pc_symm_spec->link_symmetries.top();

  while (!nil_ck_Sesp (vbls_sp)) {
    Sesp let_vals_sp = let_vals_list_sp;
    Cx::Table<uint> vbl_idcs;
    while (!nil_ck_Sesp (let_vals_sp)) {
      Sesp let_name_sp = let_names_sp;
      Sesp let_val_sp = car_of_Sesp (let_vals_sp);
      while (good && !nil_ck_Sesp (let_name_sp)) {
        DoLegitLine( "Tuple must be of the same size!" )
          !nil_ck_Sesp (let_val_sp);
        if (!good)  break;

        add_scope_let(car_of_Sesp (let_name_sp), car_of_Sesp (let_val_sp));
        let_name_sp = cdr_of_Sesp (let_name_sp);
        let_val_sp = cdr_of_Sesp (let_val_sp);
      }
      DoLegitLine( "Tuples must be of the same size!" )
        nil_ck_Sesp (let_val_sp);
      if (!good)  break;

      vbl_idcs.push(pc_symm_spec->rvbl_symms.sz());
      add_access(car_of_Sesp (vbls_sp), write, random);

      let_name_sp = let_names_sp;
      while (!nil_ck_Sesp (let_name_sp)) {
        del_scope_let(car_of_Sesp (let_name_sp));
        let_name_sp = cdr_of_Sesp (let_name_sp);
      }
      let_vals_sp = cdr_of_Sesp (let_vals_sp);
    }
    if (!good)  break;

    Cx::String index_expression;
    string_expression(index_expression, caddar_of_Sesp (vbls_sp));
    link_symmetry.add_link_symmetry(vbl_idcs, index_expression);
    vbls_sp = cdr_of_Sesp (vbls_sp);
  }
  return update_allgood (good);
}

  bool
ProtoconFile::parse_action(Cx::PFmla& act_pf, Cx::Table<Cx::PFmla>& pc_xns, Sesp act_sp,
                           bool auto_iden, Xn::Vbl::ShadowPuppetRole role)
{
  if (!allgood)  return false;
  DeclLegit( good );
  Claim( pc_symm );
  const Xn::Net& topo = sys->topology;
  act_pf = false;
  pc_xns.resize(pc_symm->membs.sz());
  for (uint i = 0; good && i < pc_symm->membs.sz(); ++i) {
    pc_xns[i] = false;
  }

  const bool actto_op =
    eq_cstr ("-=>", ccstr_of_Sesp (car_of_Sesp (act_sp)));

  const Cx::String& idx_name = pc_symm_spec->idx_name;
  for (uint pcidx = 0; good && pcidx < pc_symm->membs.sz(); ++pcidx) {
    const Xn::Pc& pc = *pc_symm->membs[pcidx];
    index_map[idx_name] = pcidx;
    Cx::PFmla guard_pf;
    DoLegitLine( "eval guard" )
      eval(guard_pf, cadr_of_Sesp (act_sp));
    if (!good)  continue;

    Cx::PFmla assign_pf( true );

    Cx::BitTable wvbl_assigned( pc.wvbls.sz(), 0 );

    bool all_wild = false;
    for (Sesp assign_sp = cddr_of_Sesp (act_sp);
         !nil_ck_Sesp (assign_sp);
         assign_sp = cdr_of_Sesp (assign_sp))
    {
      Sesp sp = car_of_Sesp (assign_sp);
      Claim( list_ck_Sesp (sp) );

      if (eq_cstr ("wild", ccstr_of_Sesp (car_of_Sesp (sp)))) {
        auto_iden = true;
        continue;
      }
      Claim( eq_cstr (":=", ccstr_of_Sesp (car_of_Sesp (sp))) );

      Sesp vbl_sp = cadr_of_Sesp (sp);
      Sesp val_sp = caddr_of_Sesp (sp);

      if (eq_cstr ("wild", ccstr_of_Sesp (car_of_Sesp (vbl_sp)))) {
        Claim( eq_cstr ("wild", ccstr_of_Sesp (car_of_Sesp (val_sp))) );
        all_wild = true;
        continue;
      }

      Xn::Vbl* vbl = 0;
      Cx::IntPFmla val;

      DoLegitLine( "eval variable" )
        eval_vbl(&vbl, vbl_sp);

      bool wild = false;
      DoLegit( "eval value" )
      {
        if (list_ck_Sesp(val_sp) && eq_cstr ("wild", ccstr_of_Sesp (car_of_Sesp (val_sp)))) {
          wild = true;
        }
        else {
          good = eval(val, val_sp);
        }
      }

      DoLegit( "non-writable variable in assignment" )
      {
        bool found = false;
        for (uint i = 0; i < pc.wvbls.sz(); ++i) {
          if (pc.wvbls[i] == vbl) {
            found = true;
            wvbl_assigned[i] = 1;
            break;
          }
        }
        good = found;
      }
      if (!good)  break;

      if (all_wild) {
        assign_pf = true;
      }
      else if (!wild) {
        const Cx::PFmlaVbl& pf_vbl = topo.pfmla_vbl(*vbl);
        val %= vbl->symm->domsz;
        assign_pf &= pf_vbl.img_eq(val);
      }

    }

    for (uint i = 0; i < pc.wvbls.sz(); ++i) {
      if (all_wild) {
        wvbl_assigned[i] = true;
      }
      else if (actto_op) {
        // The {-=>} operator automatically randomizes values.
        if (pc_symm_spec->random_write_flags[pc_symm->wmap[i]]) {
          wvbl_assigned[i] = true;
        }
      }
      if (!auto_iden || wvbl_assigned[i]) {
        continue;
      }
      if (role != Xn::Vbl::Shadow && pc_symm->wvbl_symms[i]->pure_shadow_ck()) {
        DBog0( "All writable shadow variables must be assigned in a puppet action!" );
        good = false;
      }
      else {
        const Cx::PFmlaVbl& pf_vbl = topo.pfmla_vbl(*pc.wvbls[i]);
        assign_pf &= pf_vbl.img_eq(pf_vbl);
      }
    }

    pc_xns[pcidx] = guard_pf & assign_pf;

    if (actto_op) {
      X::Fmla self_xn( true );
      for (uint i = 0; i < pc.wvbls.sz(); ++i) {
        if (!wvbl_assigned[i]) {
          continue;
        }
        if (pc_symm_spec->random_write_flags[pc_symm->wmap[i]]) {
          continue;
        }
        if (role != Xn::Vbl::Shadow && pc_symm->wvbl_symms[i]->pure_shadow_ck()) {
          continue;
        }
        const Cx::PFmlaVbl& pf_vbl = topo.pfmla_vbl(*pc.wvbls[i]);
        self_xn &= pf_vbl.img_eq(pf_vbl);
      }
      pc_xns[pcidx] -= self_xn;
    }
    if (pc_symm->pure_shadow_ck()) {
      X::Fmla self_xn( true );
      for (uint i = 0; i < pc.wvbls.sz(); ++i) {
        const Cx::PFmlaVbl& pf_vbl = topo.pfmla_vbl(*pc.wvbls[i]);
        self_xn &= pf_vbl.img_eq(pf_vbl);
      }
      pc_xns[pcidx] -= self_xn;
    }

    act_pf |= pc_xns[pcidx] & pc.act_unchanged_pfmla;
  }
  if (good)
    index_map.erase(idx_name);
  return update_allgood (good);
}

  bool
ProtoconFile::add_action(Sesp act_sp, Xn::Vbl::ShadowPuppetRole role)
{
  if (!allgood)  return false;
  DeclLegit( good );
  Claim( pc_symm );
  Xn::Net& topo = sys->topology;

  DoLegit( "" )
  {
    if (role != Xn::Vbl::Puppet) {
      Cx::String act_expression;
      good = string_expression (act_expression, act_sp);
      if (good) {
        pc_symm_spec->shadow_act_strings.push(act_expression);
      }
    }
  }

  if (!this->interpret_ck())  return update_allgood (good);

  Cx::PFmla act_pf( false );
  Cx::Table<Cx::PFmla> pc_xns;
  DoLegitLine( "parse action" )
    parse_action(act_pf, pc_xns, act_sp, true, role);

  if (good) {
    for (uint i = 0; i < pc_symm->membs.sz(); ++i) {
      if (role != Xn::Vbl::Puppet) {
        pc_symm->membs[i]->shadow_xn |= topo.proj_shadow(pc_xns[i]);
      }
      if (role != Xn::Vbl::Shadow) {
        pc_symm->membs[i]->puppet_xn |= pc_xns[i];
      }
    }
  }

  if (false)
  DoLegitLine("self-loop")
    !act_pf.overlap_ck(topo.identity_xn);

  DoLegit("")
  {
    if (role != Xn::Vbl::Puppet) {
      const Cx::PFmla& shadow_act_pf =
        (topo.smooth_pure_puppet_vbls(act_pf) -
         topo.smooth_pure_puppet_vbls(topo.identity_xn));
      pc_symm->shadow_pfmla |= shadow_act_pf;
      sys->shadow_pfmla |= shadow_act_pf;
    }


    if (role != Xn::Vbl::Shadow) {
#if 0
      uint rep_pcidx = 0;
      pc_symm->representative(&rep_pcidx);
      pc_symm->direct_pfmla |= pc_xns[rep_pcidx];
#else
      pc_symm->direct_pfmla |= act_pf;
#endif
      sys->direct_pfmla |= act_pf;
    }
  }

  return update_allgood (good);
}

  bool
ProtoconFile::forbid_action(Sesp act_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );
  Claim( pc_symm );

  DoLegit( "" )
  {
    Cx::String act_expression;
    good = string_expression (act_expression, act_sp);
    if (good) {
      pc_symm_spec->forbid_act_strings.push(act_expression);
    }
  }

  if (!this->interpret_ck())  return update_allgood (good);

  Cx::PFmla act_pf( false );
  Cx::Table<Cx::PFmla> pc_xns;
  DoLegitLine( "parse action" )
    parse_action(act_pf, pc_xns, act_sp, false, Xn::Vbl::Puppet);

  DoLegit( "" ) {
    for (uint i = 0; i < pc_symm->membs.sz(); ++i) {
      pc_symm->membs[i]->forbid_xn |= pc_xns[i];
    }
  }

  return update_allgood (good);
}

  bool
ProtoconFile::permit_action(Sesp act_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );
  Claim( pc_symm );

  DoLegit( "" )
  {
    Cx::String act_expression;
    good = string_expression (act_expression, act_sp);
    if (good) {
      pc_symm_spec->permit_act_strings.push(act_expression);
    }
  }

  if (!this->interpret_ck())  return update_allgood (good);

  Cx::PFmla act_pf( false );
  Cx::Table<Cx::PFmla> pc_xns;
  DoLegitLine( "parse action" )
    parse_action(act_pf, pc_xns, act_sp, false, Xn::Vbl::Puppet);

  DoLegit( "" ) {
    for (uint i = 0; i < pc_symm->membs.sz(); ++i) {
      pc_symm->membs[i]->permit_xn |= pc_xns[i];
    }
  }

  return update_allgood (good);
}

  bool
ProtoconFile::add_pc_predicate(Sesp name_sp, Sesp val_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );

  if (!this->interpret_ck())  return update_allgood (good);

  const char* name;
  DoLegitLineP( name, "" )
    ccstr_of_Sesp (name_sp);

  const Cx::String& idx_name = pc_symm_spec->idx_name;
  Xn::NatPredicateMap let_vals( pc_symm->membs.sz() );
  for (uint i = 0; good && i < pc_symm->membs.sz(); ++i) {
    index_map[idx_name] = i;
    DoLegitLine( "" )
      eval (let_vals.membs[i], val_sp);
  }
  index_map.erase(idx_name);

  DoLegitLine( "finding expression" )
    string_expression (let_vals.expression, val_sp);
  DoLegit( "" )
    pc_symm->predicate_map.add(name, let_vals);
  return update_allgood (good);
}

  bool
ProtoconFile::add_pc_assume(Sesp assume_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );

  Cx::String assume_expression;
  DoLegitLine( "" )
    parend_string_expression(assume_expression, assume_sp);

  DoLegit( "" ) {
    if (!pc_symm_spec->closed_assume_expression.empty_ck()) {
      pc_symm_spec->closed_assume_expression << " && ";
    }
    pc_symm_spec->closed_assume_expression << assume_expression;
  }

  if (!this->interpret_ck())  return update_allgood (good);

  const Cx::String& idx_name = pc_symm_spec->idx_name;
  Cx::PFmla pf;

  for (uint i = 0; i < pc_symm->membs.sz(); ++i) {
    index_map[idx_name] = i;
    DoLegitLine( "" )
      eval(pf, assume_sp);

    if (!good)  break;
    pc_symm->membs[i]->closed_assume &= pf;
    sys->closed_assume &= pf;
  }

  index_map.erase(idx_name);
  return update_allgood (good);
}

  bool
ProtoconFile::add_pc_legit(Sesp legit_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );

  Cx::String invariant_expression;
  DoLegitLine( "" )
    string_expression(invariant_expression, legit_sp);

  DoLegit( "" )
  {
    if (!pc_symm_spec->invariant_expression.empty_ck()) {
      pc_symm_spec->invariant_expression =
        Cx::String("(") + pc_symm_spec->invariant_expression + ") && ";
    }
    pc_symm_spec->invariant_expression += invariant_expression;
  }

  if (!this->interpret_ck())  return update_allgood (good);

  const Cx::String& idx_name = pc_symm_spec->idx_name;
  Cx::PFmla pf;

  for (uint i = 0; i < pc_symm->membs.sz(); ++i) {
    index_map[idx_name] = i;
    DoLegitLine( "" )
      eval(pf, legit_sp);

    DoLegit( "" )
    {
      sys->invariant &= pf;
      pc_symm->membs[i]->invariant &= pf;
    }
  }

  index_map.erase(idx_name);
  return update_allgood (good);
}

  bool
ProtoconFile::finish_pc_def()
{
  bool good = true;
  this->pc_symm = 0;
  this->pc_symm_spec = 0;
  return update_allgood (good);
}

  bool
ProtoconFile::add_predicate(Sesp name_sp, Sesp val_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );
  const char* name;

  DoLegitLineP( name, "" )
    ccstr_of_Sesp (name_sp);

  Cx::String expression;
  DoLegitLine( "finding expression" )
    string_expression (expression, val_sp);


  Cx::PFmla pf(false);
  if (this->interpret_ck()) {
    DoLegitLine( 0 )
      eval(pf, val_sp);
  }

  DoLegit( 0 )
    sys->predicate_map.add(name, pf, expression);

  return update_allgood (good);
}

  bool
ProtoconFile::add_assume(Sesp assume_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );

  Cx::String str;
  DoLegitLine( "convert invariant expression to string" )
    parend_string_expression(str, assume_sp);

  DoLegit(0) {
    if (spec->closed_assume_expression != "") {
      spec->closed_assume_expression << "\n  &&\n  ";
    }
    spec->closed_assume_expression << str;
  }
  if (!this->interpret_ck())  return update_allgood (good);

  Cx::PFmla pf;
  DoLegitLine( "parse invariant" )
    eval(pf, assume_sp);

  DoLegit(0)
    sys->closed_assume &= pf;

  return update_allgood (good);
}

  bool
ProtoconFile::assign_legit_mode(Xn::InvariantStyle style, Xn::InvariantScope scope)
{
  if (!allgood)  return false;
  DeclLegit( good );
  if (legit_mode_assigned) {
    DoLegitLine( "Invariant must have the same style in all places." )
      (spec->invariant_style == style &&
       spec->invariant_scope == scope);
  }
  spec->invariant_style = style;
  spec->invariant_scope = scope;
  legit_mode_assigned = true;
  return update_allgood (good);
}

  bool
ProtoconFile::add_legit(Sesp legit_sp)
{
  if (!allgood)  return false;
  DeclLegit( good );

  Cx::String invariant_expression;
  DoLegitLine( "convert invariant expression to string" )
    string_expression(invariant_expression, legit_sp);

  DoLegit(0) {
    if (spec->invariant_expression != "") {
      spec->invariant_expression =
        Cx::String("(") + spec->invariant_expression + ")\n  &&\n  ";
    }
    spec->invariant_expression += invariant_expression;
  }
  if (!this->interpret_ck())  return update_allgood (good);


  Cx::PFmla pf;
  DoLegitLine( "parse invariant" )
    eval(pf, legit_sp);

  DoLegit(0)
    sys->invariant &= pf;

  return update_allgood (good);
}

  bool
ProtoconFile::string_expression(Cx::String& ss, Sesp a)
{
  if (!allgood)  return false;
  DeclLegit( good );

  const char* key = 0;

  DoLegitLine( "" )
    !!a;

  DoLegitP( key, "ccstr_of_Sesp()" )
  {
    const char* name = ccstr_of_Sesp (a);
    if (name)
    {
      {
        Sesp* val_sp = scope_let_map.lookup(name);
        if (val_sp) {
          return string_expression(ss, *val_sp);
        }
      }
      ss += name;
      return good;
    }

    uint u = 0;
    if (uint_of_Sesp (a, &u))
    {
      ss += u;
      return good;
    }

    key = ccstr_of_Sesp (car_of_Sesp (a));
  }

  if (!good) {
  }
  else if (eq_cstr (key, "(bool)") ||
           eq_cstr (key, "(int)")) {
    ss += "(";
    string_expression(ss, cadr_of_Sesp (a));
    ss += ")";
  }
  else if (eq_cstr (key, "!")) {
    ss += "!";
    string_expression (ss, cadr_of_Sesp (a));
  }
  else if (eq_cstr (key, "negate")) {
    ss += "-";
    string_expression (ss, cadr_of_Sesp (a));
  }
  else if (eq_cstr (key, "&&") ||
           eq_cstr (key, "||") ||
           eq_cstr (key, "==>") ||
           eq_cstr (key, "<=>") ||
           eq_cstr (key, "<") ||
           eq_cstr (key, "<=") ||
           eq_cstr (key, "!=") ||
           eq_cstr (key, "==") ||
           eq_cstr (key, ">=") ||
           eq_cstr (key, ">") ||
           eq_cstr (key, ":=") ||
           eq_cstr (key, "+") ||
           eq_cstr (key, "-") ||
           eq_cstr (key, "*") ||
           eq_cstr (key, "/") ||
           eq_cstr (key, "%") ||
           eq_cstr (key, "^")) {
    bool pad =
      (eq_cstr (key, "&&") ||
       eq_cstr (key, "||") ||
       eq_cstr (key, "==>") ||
       eq_cstr (key, "<=>"));
    string_expression (ss, cadr_of_Sesp (a));
    if (pad)  ss += " ";
    ss += key;
    if (pad)  ss += " ";
    string_expression (ss, caddr_of_Sesp (a));
  }
  else if (eq_cstr (key, "xor")) {
    string_expression (ss, cadr_of_Sesp (a));
    ss += "!=";
    string_expression (ss, caddr_of_Sesp (a));
  }
  else if (eq_cstr (key, "-->") ||
           eq_cstr (key, "-=>")) {
    string_expression (ss, cadr_of_Sesp (a));
    ss << " " << key;
    for (Sesp b = cddr_of_Sesp (a); !nil_ck_Sesp (b); b = cdr_of_Sesp (b)) {
      ss += " ";
      string_expression (ss, car_of_Sesp (b));
      ss += ";";
    }
  }
  else if (eq_cstr (key, "min") ||
           eq_cstr (key, "max")) {
    ss << key << "(";
    string_expression (ss, cadr_of_Sesp (a));
    ss << ",";
    string_expression (ss, caddr_of_Sesp (a));
    ss << ")";
  }
  else if (eq_cstr (key, "wild"))
  {
    ss += "_";
  }
  else if (eq_cstr (key, "NatDom")) {
    ss += "Nat % ";
    string_expression (ss, cadr_of_Sesp (a));
  }
  else if (eq_cstr (key, "aref")) {
    ss += ccstr_of_Sesp (cadr_of_Sesp (a));
    ss += "[";
    string_expression (ss, caddr_of_Sesp (a));
    ss += "]";
  }
  else if (eq_cstr (key, "iif")) {
    ss += "if ";
    string_expression (ss, cadr_of_Sesp (a));
    ss += " then ";
    string_expression (ss, caddr_of_Sesp (a));
    ss += " else ";
    string_expression (ss, cadddr_of_Sesp (a));
  }
  else if (eq_cstr (key, "forall") ||
           eq_cstr (key, "exists") ||
           eq_cstr (key, "unique")
          )
  {
    ss += key;
    ss += " ";
    ss += ccstr_of_Sesp (cadr_of_Sesp (a));
    ss += " <- ";
    string_expression (ss, caddr_of_Sesp (a));
    ss += " : ";
    string_expression (ss, cadddr_of_Sesp (a));
  }
  else {
    good = false;
    DBog1( "No matching string, key is: \"%s\"", key );
  }

  return update_allgood (good);
}

  bool
ProtoconFile::parend_string_expression(Cx::String& ss, Sesp a)
{
  const char* key = 0;
  if (list_ck_Sesp (a)) {
    key = ccstr_of_Sesp (car_of_Sesp (a));
  }
  bool wrap = (!eq_cstr (key, "(bool)") &&
               !eq_cstr (key, "(int)"));
  if (wrap)  ss << '(';
  bool good = string_expression(ss, a);
  if (wrap)  ss << ')';
  return good;
}

  bool
ProtoconFile::eval(Cx::PFmla& pf, Sesp a)
{
  if (!allgood)  return false;
  DeclLegit( good );

  DoLegitLine("")
    !!a;

  DoLegit(0) {
    const char* name = ccstr_of_Sesp (a);
    if (name)
    {
      good = lookup_pfmla(&pf, name);
      if (!good)
        bad_parse(name, "Unknown predicate name.");
      return update_allgood(good);
    }
  }

  Sesp b = cdr_of_Sesp (a);
  Sesp c = cdr_of_Sesp (b);
  Sesp d = cdr_of_Sesp (c);

  a = car_of_Sesp (a);
  b = car_of_Sesp (b);
  c = car_of_Sesp (c);
  d = car_of_Sesp (d);

  const char* key = ccstr_of_Sesp (a);

  // Temporaries.
  Cx::IntPFmla ipf_b;
  Cx::IntPFmla ipf_c;
  Cx::PFmla pf_b;
  Cx::PFmla pf_c;

  //DBog1("key is: %s", key);

  if (eq_cstr (key, "(bool)") ||
      eq_cstr (key, "(int)" ))
  {
    DoLegitLine("")
      eval(pf, b);
  }
  else if (eq_cstr (key, "!")) {
    DoLegitLine("")
      eval(pf, b);
    DoLegit(0)
      pf = !pf;
  }
  else if (eq_cstr (key, "&&")) {
    DoLegitLine("")
      eval(pf, b);

    DoLegit(0) {
      if (!pf.sat_ck()) {
        // Short circuit.
      }
      else {
        DoLegitLine("")
          eval(pf_c, c);
        if (good)
          pf &= pf_c;
      }
    }
  }
  else if (eq_cstr (key, "||")) {
    DoLegitLine("")
      eval(pf, b);

    DoLegit(0) {
      if (pf.tautology_ck()) {
        // Short circuit.
      }
      else {
        DoLegitLine("")
          eval(pf_c, c);
        if (good)
          pf |= pf_c;
      }
    }
  }
  else if (eq_cstr (key, "==>")) {
    DoLegitLine("")
      eval(pf, b);
    DoLegit(0) {
      if (!pf.sat_ck()) {
        // Short circuit.
        pf = true;
      }
      else {
        DoLegitLine("")
          eval(pf_c, c);
        if (good)
          pf = ~pf | pf_c;
      }
    }
  }
  else if (eq_cstr (key, "<=>") ||
           eq_cstr (key, "xor")
          )
  {
    DoLegitLine("")
      eval(pf, b);
    DoLegitLine("")
      eval(pf_c, c);

    if (!good)
      /* Nothing.*/;
    else if (eq_cstr (key, "<=>"))
      pf.defeq_xnor(pf_c);
    else if (eq_cstr (key, "xor"))
      pf ^= pf_c;
    else
      Claim( 0 && "Missing case." );
  }
  else if (eq_cstr (key, "<" ) ||
           eq_cstr (key, "<=") ||
           eq_cstr (key, "!=") ||
           eq_cstr (key, "==") ||
           eq_cstr (key, ">=") ||
           eq_cstr (key, ">" ))
  {
    DoLegitLine("")
      eval(ipf_b, b);
    DoLegitLine("")
      eval(ipf_c, c);

    DoLegit(0) {
      if      (eq_cstr (key, "<" ))  pf = (ipf_b < ipf_c);
      else if (eq_cstr (key, "<="))  pf = (ipf_b <= ipf_c);
      else if (eq_cstr (key, "!="))  pf = (ipf_b != ipf_c);
      else if (eq_cstr (key, "=="))  pf = (ipf_b == ipf_c);
      else if (eq_cstr (key, ">="))  pf = (ipf_b >= ipf_c);
      else if (eq_cstr (key, ">" ))  pf = (ipf_b > ipf_c);
      else Claim(0 && "Missing case.");
    }
  }
  else if (eq_cstr (key, "forall") ||
           eq_cstr (key, "exists") ||
           eq_cstr (key, "unique")
          )
  {
    const char* idxname = ccstr_of_Sesp (b);
    DoLegitLine("")
      !!idxname;
    DoLegitLine("")
      eval(ipf_c, c);
    DoLegitLine("")
      ipf_c.vbls.sz() == 0;

    DoLegit("") {
      const uint n = ipf_c.state_map.sz();

      if (eq_cstr (key, "forall")) {
        pf = true;
      }
      else if (eq_cstr (key, "exists")) {
        pf = false;
      }
      else {
        pf = false;
        pf_b = false;
      }
#if 1
      for (uint i = 0; good && i < n; ++i) {
        index_map[idxname] = ipf_c.state_map[i];
        DoLegitLine("")
          eval(pf_c, d);
        DoLegit("") {
          if (eq_cstr (key, "forall")) {
            pf &= pf_c;
          }
          else if (eq_cstr (key, "exists")) {
            pf |= pf_c;
          }
          else {
            pf -= (pf_c & pf_b);
            pf |= (pf_c - pf_b);
            pf_b |= pf_c;
          }
        }
      }
      index_map.erase(idxname);
#else
      Cx::Table< Cx::PFmla > pfmlas( n );

      for (uint i = 0; good && i < n; ++i) {
        index_map[idxname] = ipf_c.state_map[i];
        DoLegitLine("")
          eval(pfmlas[i], d);
      }

      if (good) {
        if (eq_cstr (key, "forall")) {
          pf = true;
          for (uint i = 0; i < n; ++i) {
            pf &= pfmlas[i];
          }
        }
        else if (eq_cstr (key, "exists")) {
          pf = false;
          for (uint i = 0; i < n; ++i) {
            pf |= pfmlas[i];
          }
        }
        else {
          pf = false;
          for (uint i = 0; i < n; ++i) {
            Cx::PFmla conj( pfmlas[i] );
            for (uint j = 0; j < n; ++j) {
              if (j != i) {
                conj &= ~ pfmlas[j];
              }
            }
            pf |= conj;
          }
        }
      }

      index_map.erase(idxname);
#endif
    }
  }
  else {
    good = false;
    DBog1( "No matching string, key is: \"%s\"", key );
  }

  return update_allgood (good);
}


/**
 * Second eval() function.
 * This one is for IntPFmlas.
 **/
  bool
ProtoconFile::eval(Cx::IntPFmla& ipf, Sesp a)
{
  if (!allgood)  return false;
  DeclLegit( good );

  DoLegitLine( "" )
    !!a;

  DoLegit( "" )
  {
    const char* name = ccstr_of_Sesp (a);
    if (name)
    {
      Sesp* val_sp = scope_let_map.lookup(name);
      if (val_sp) {
        DoLegitLine( "evaluating scope'd let" )
          eval(ipf, *val_sp);
        return update_allgood (good);
      }

      int val = 0;
      DoLegitLine( "lookup_int()" )
        lookup_int(&val, name);

      DoLegit(0)
        ipf = Cx::IntPFmla( val );
      return update_allgood (good);
    }

    uint x = 0;
    if (uint_of_Sesp (a, &x)) {
      ipf = Cx::IntPFmla( x );
      //DBog1("int is: %u", x);
      return update_allgood (good);
    }
  }

  if (!good)
    return update_allgood (good);

  Sesp b = cdr_of_Sesp (a);
  Sesp c = cdr_of_Sesp (b);
  Sesp d = cdr_of_Sesp (c);

  a = car_of_Sesp (a);
  b = car_of_Sesp (b);
  c = car_of_Sesp (c);
  d = car_of_Sesp (d);

  const char* key = ccstr_of_Sesp (a);

  Cx::IntPFmla ipf_c;

  DoLegitLine("car_of_Sesp()")
    !!a;
  DoLegitLine("ccstr_of_Sesp()")
    !!key;

  if (false && !good)
  {
    if (nil_ck_Sesp (a))
      DBog0("it's nil");
    if (list_ck_Sesp (a))
      DBog0("it's a list");
    if (atom_ck_Sesp (a))
      DBog0("it's an atom");
    if (a) {
      DBog1( "a->kind->vt->kind_name = %s", a->kind->vt->kind_name );
    }
  }

  if (!good)
  {}
  else if (eq_cstr (key, "(int)")) {
    DoLegitLine("")
      eval(ipf, b);
  }
  else if (eq_cstr (key, "negate")) {
    DoLegitLine("")
      eval(ipf, b);
    DoLegit(0)
      ipf.negate();
  }
  else if (eq_cstr (key, "+") ||
           eq_cstr (key, "-") ||
           eq_cstr (key, "*") ||
           eq_cstr (key, "/") ||
           eq_cstr (key, "%") ||
           eq_cstr (key, "^") ||
           eq_cstr (key, "min") ||
           eq_cstr (key, "max")
          ) {
    DoLegitLine("")
      eval(ipf, b);
    DoLegitLine("")
      eval(ipf_c, c);
    if (!good)
      /*Nothing*/;
    else if (eq_cstr (key, "+"))
      ipf += ipf_c;
    else if (eq_cstr (key, "-"))
      ipf -= ipf_c;
    else if (eq_cstr (key, "*"))
      ipf *= ipf_c;
    else if (eq_cstr (key, "/"))
      ipf /= ipf_c;
    else if (eq_cstr (key, "%"))
      ipf %= ipf_c;
    else if (eq_cstr (key, "^"))
      ipf.defeq_pow(ipf_c);
    else if (eq_cstr (key, "min"))
      ipf.defeq_min(ipf_c);
    else if (eq_cstr (key, "max"))
      ipf.defeq_max(ipf_c);
    else
      Claim(0 && "Missed a case!");
  }
  else if (eq_cstr (key, "NatDom")) {
    uint domsz = 0;
    DoLegitLine("")
      eval_gtz (&domsz, b);
    DoLegit(0)
      ipf = Cx::IntPFmla( 1, 0, domsz );
  }
  else if (eq_cstr (key, "iif")) {
    Cx::PFmla pf( true );
    DoLegitLine("")
      eval(pf, b);

    DoLegit("") {
      if (pf.tautology_ck()) {
        b = c;
      }
      else if (!pf.sat_ck()) {
        b = d;
      }
      else {
        DBog0("The conditional is not constant!");
        good = false;
      }
    }
    DoLegitLine("")
      eval(ipf, b);
  }
  else if (eq_cstr (key, "aref")) {
    Xn::Vbl* vbl = 0;
    DoLegitLine("")
      eval_vbl(&vbl, b, c);
    DoLegit(0)
      ipf = Cx::IntPFmla(sys->topology.pfmla_vbl(*vbl));
  }
  else {
    good = false;
    DBog1( "No matching string, key is: \"%s\"", key );
  }

  return update_allgood (good);
}

  bool
ProtoconFile::eval_int(int* ret, Sesp sp)
{
  DeclLegit( good );
  Cx::IntPFmla ipf;
  DoLegitLine("")
    eval(ipf, sp);

  DoLegitLine("")
    ipf.state_map.sz() == 1;

  DoLegit(0)
    *ret = ipf.state_map[0];
  return update_allgood (good);
}

  bool
ProtoconFile::eval_nat(uint* ret, Sesp sp)
{
  int x = 0;
  DeclLegit( good );
  DoLegitLine("")
    eval_int (&x, sp);

  DoLegitLine("cannot be negative")
    x >= 0;

  DoLegit(0)
    *ret = (uint) x;
  return update_allgood (good);
}

  bool
ProtoconFile::eval_gtz(uint* ret, Sesp sp)
{
  int x = 0;
  DeclLegit( good );
  DoLegitLine("")
    eval_int (&x, sp);
  DoLegitLine("must be positive")
    x > 0;
  DoLegit(0)
    *ret = (uint) x;
  return update_allgood (good);
}

  bool
ProtoconFile::eval_vbl(Xn::Vbl** ret, Sesp a)
{
  Claim( eq_cstr ("aref", ccstr_of_Sesp (car_of_Sesp (a))) );
  return eval_vbl(ret, cadr_of_Sesp (a), caddr_of_Sesp (a));
}

  bool
ProtoconFile::eval_vbl(Xn::Vbl** ret, Sesp b, Sesp c)
{
  DeclLegit( good );

  DoLegitLine( "null variable or index?!" )
    b && c;

  const char* name = 0;
  DoLegit( "variable needs name" )
  {
    name = ccstr_of_Sesp (b);
    good = !!name;
  }

  const Xn::VblSymm* symm = 0;
  DoLegit( "variable lookup" )
  {
    const Xn::VblSymm** tmp = vbl_map.lookup(name);
    good = !!tmp;
    if (tmp)
      symm = *tmp;
  }

  int idx = 0;
  DoLegitLine( "eval array index" )
    eval_int(&idx, c);

  DoLegit(0)
    *ret = symm->membs[umod_int (idx, symm->membs.sz())];

  DoLegit( "process needs read access to variable" ) {
    if ((bool)pc_symm) {
      const int* pcidx = index_map.lookup(pc_symm_spec->idx_name);
      if (pcidx) {
        const Xn::Pc& pc = *pc_symm->membs[*pcidx];
        bool found = false;
        for (uint i = 0; !found && i < pc.rvbls.sz(); ++i) {
          found = (pc.rvbls[i] == *ret);
        }
        good = found;
      }
    }
  }

  if (!good)
    *ret = 0;
  return update_allgood (good);
}

  bool
ProtoconFile::lookup_pfmla(Cx::PFmla* ret, const Cx::String& name)
{
  const Cx::PFmla* pf = sys->predicate_map.lookup(name);
  if (pf) {
    *ret = *pf;
    return true;
  }
  if ((bool)pc_symm) {
    const int* pcidx = index_map.lookup(pc_symm_spec->idx_name);
    if (pcidx) {
      Xn::NatPredicateMap* vals = pc_symm->predicate_map.lookup(name);
      if (vals) {
        *ret = vals->eval(*pcidx);
        return true;
      }
    }
  }

  bad_parse(name.ccstr(), "Unknown predicate name.");
  return false;
}

  bool
ProtoconFile::lookup_int(int* ret, const Cx::String& name)
{
  const int* val = index_map.lookup(name);
  if (val) {
    *ret = *val;
    return true;
  }

  if (spec->constant_map.key_ck(name)) {
    *ret = spec->constant_map.lookup(name)->membs[0];
    return true;
  }
  if ((bool)pc_symm) {
    const int* pcidx = index_map.lookup(pc_symm_spec->idx_name);
    if (pcidx) {
      Xn::NatMap* vals = pc_symm_spec->let_map.lookup(name);
      if (vals) {
        *ret = vals->eval(*pcidx);
        return true;
      }
    }
  }
  return update_allgood (false);
}
