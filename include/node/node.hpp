#pragma once

#include "base.hpp"

#include <cspace/processor.hpp>

namespace node {
  struct color : public meta {
    using meta::meta;
    cspace::processor processor;

    string get  () const;
    base_p clone  (clone_context&) const;
  };

  struct env : public meta, settable {
    using meta::meta;
    string get  () const;
    bool set  (const string& value);
    base_p clone  (clone_context&) const;
  };

  struct cmd : public meta {
    using meta::meta;
    string get  () const;
    base_p clone  (clone_context&) const;
  };

  struct file : public meta, settable {
    using meta::meta;
    string get  () const;
    bool set  (const string& value);
    base_p clone  (clone_context&) const;
  };

  struct map : public meta {
    using meta::meta;
    float from_min{0}, from_range{0}, to_min{0}, to_range{0};

    string get  () const;
    base_p clone  (clone_context&) const;
  };

  struct string_interpolate : public base {
    struct replace_spot {
      int position{0};;
      base_p replacement;
    };
    string base;
    std::vector<replace_spot> spots;

    string get  () const;
    base_p clone  (clone_context&) const;
  };
}
