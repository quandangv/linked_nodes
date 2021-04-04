#pragma once

#include "base.hpp"
#include "fallback.hpp"

#include <chrono>
#include <cspace/processor.hpp>
#include <cspace/gradient.hpp>
#include <poll.h>

namespace node {
  using steady_time = std::chrono::time_point<std::chrono::steady_clock>;

    template<class T>
  struct nested {
    std::shared_ptr<base<T>> value;

    nested(parse_context& context, parse_preprocessed& prep);
    nested(const nested<T>& other, clone_context& context);
    nested() {}
  };

  struct meta : base<string>, nested<string>, with_fallback<string> {
    bool is_fixed() const { return value->is_fixed(); }
    meta(parse_context& context, parse_preprocessed& prep)
        : nested(context, prep)
        , with_fallback(prep.pop_fallback()) {}
    meta(const meta& other, clone_context& context)
        : nested(other, context)
        , with_fallback(other.fallback ? other.fallback->clone(context) : base_s()) {}
  };

  struct simple_meta : meta {
    using meta::meta;
    simple_meta(parse_context& context, parse_preprocessed& prep) : meta(context, prep) {
      if (prep.token_count != 2)
        throw parse_error("parse_error: Only accept 1 component");
    }
  };

  struct color : meta {
    cspace::processor processor;

    color(parse_context&, parse_preprocessed&);
    explicit operator string() const;
    base_s clone(clone_context&) const;
  protected:
    using meta::meta;
  };

  template<class From, class To, class Processor>
  struct loaded_node : base<To>, nested<From> {
    mutable Processor base;

    explicit operator string() const;
    virtual Processor& get_base() const { return base; }
    bool is_fixed() const { return nested<From>::value->is_fixed(); }

    base_s clone(clone_context&) const
    { throw clone_error("clone_error: This node is optimized and can't be cloned"); }
  protected:
    using nested<float>::nested;
  };

  template<class From, class To, class Processor>
  struct lazy_node : loaded_node<From, To, Processor> {
    base_s base_raw;

    base_s clone(clone_context&) const;
    lazy_node(parse_context&, parse_preprocessed&);
  protected:
    using loaded_node<From, To, Processor>::loaded_node;
  };

  struct gradient : lazy_node<float, string, cspace::gradient<3>> {
    cspace::gradient<3>& get_base() const;
  protected:
    using lazy_node<float, string, cspace::gradient<3>>::lazy_node;
  };

  struct env : simple_meta, settable<string> {
    explicit operator string() const;
    bool set  (const string& value);
    base_s clone(clone_context&) const;
    bool is_fixed() const { return false; }
  protected:
    using simple_meta::simple_meta;
  };

  struct cmd : simple_meta {
    explicit operator string() const;
    base_s clone(clone_context&) const;
    bool is_fixed() const { return false; }
  protected:
    using simple_meta::simple_meta;
  };

  struct poll : simple_meta, settable<string> {
    mutable pollfd pfd{0, POLLIN, 0};

    using simple_meta::simple_meta;
    ~poll();
    explicit operator string() const;
    base_s clone(clone_context&) const;
    void start_cmd() const;
    bool is_fixed() const { return false; }
    bool set(const string& value);
  };

  struct file : simple_meta, settable<string> {
    explicit operator string() const;
    bool set(const string& value);
    base_s clone(clone_context&) const;
    bool is_fixed() const { return false; }
  protected:
    using simple_meta::simple_meta;
  };

  struct save : base<string>, settable<string> {
    base_s value;
    base_s target;
    char delimiter{'\n'};

    save(parse_context&, parse_preprocessed&);
    save() {}
    explicit operator string() const;
    base_s clone(clone_context&) const;
    bool set(const string&);
  };

  struct map : base<float> {
    std::shared_ptr<base<float>> value;
    float from_min{0}, from_range{0}, to_min{0}, to_range{0};

    explicit operator float() const;
    base_s clone(clone_context&) const;
    bool is_fixed() const { return value->is_fixed(); }

      static std::shared_ptr<map>
    parse(parse_context&, parse_preprocessed&);
  };

  struct smooth : base<float> {
    std::shared_ptr<base<float>> value;
    float spring, drag;
    mutable float current{0}, velocity{0};

    explicit operator float() const;
    base_s clone(clone_context&) const;
    bool is_fixed() const { return value->is_fixed(); }

      static std::shared_ptr<smooth>
    parse(parse_context&, parse_preprocessed&);
  };

  struct clock : base<int> {
    std::chrono::milliseconds tick_duration;
    unsigned int loop;
    mutable steady_time zero_point;

    explicit operator int() const;
    base_s clone  (clone_context&) const;

      static std::shared_ptr<clock>
    parse(parse_context&, parse_preprocessed&);
  };
}
