#!/bin/env ruby
# encoding: utf-8

require 'fileutils'
require 'rainbow/ext/string'
require 'json'

# A collector to collect the dependecy of all the objects automatically.
class DependCollector
  DEFAULT_OPTS = { cc: 'g++', cflag: '-std=c++0x', inc: '' }

  def initialize(file, opts)
    @depend = {}
    @opts = DEFAULT_OPTS.map { |k, v| opts[k] ? [k, opts[k]] : [k, v] }.to_h
    @opts[:inc] = @opts[:inc].split(' ').reject { |x| x == '-I' }
      .map { |x| reject_i(x) }

    @opts[:cflag] += ' ' + @opts[:inc].select { |x| x[0] == '-' }.join(' ')
    @opts[:inc].reject! { |x| x[0] == '-' }
    @opts[:inc].map! { |d| File.expand_path(d) }

    ObjectSpace.define_finalizer(self,  proc { store(file) })

    return unless File.exist?(file) && !File.zero?(file)
    @depend.merge!(JSON.parse(File.new(file).read))
  end

  def list_inc(obj)
    list(obj).select { |x| File.extname(x) == '.h' }
      .map { |f| File.dirname(f) }.uniq.map { |x| match_include(x) }
      .compact.flatten.uniq.map { |f| "-I #{f}" }.join(' ')
  end

  def cflag
    @opts[:cflag]
  end

  def list(obj)
    @depend[obj.to_s] || gen_depend(obj) || raise("Dependency of file #{obj} error")
  end

  def gen_depend(obj)
    source = obj.sub('.o', '.cc')

    puts "Generating dependency for #{source}".color(:yellow)
    res = `#{@opts[:cc]} #{@opts[:cflag]} #{@opts[:inc].map { |x| "-I #{x}" }.join(' ')} -E -M -fPIC #{source}`
       .split(' ').select { |x| x != '\\' }.select { |x| necessary_depend?(x) }

    return if res.empty?
    @depend.store(obj.to_s, res[2..-1].sort.unshift(res[1]))
  end

  private

  EXCLUDE_EXT = ['', '.tcc']
  def necessary_depend?(file)
    extname = File.extname(file)
    return if EXCLUDE_EXT.include?(extname)

    return true unless ['.h', '.hpp'].include?(extname)
    match_include(File.expand_path(File.dirname(file)))
  end

  def reject_i(dir)
    dir.sub(/^-I/, '')
  end

  # These system include DIR should be excluded when compiling, or some trouble would be caused
  SYSTEM_DIR = %w(/usr/include/sys /usr/include/linux /usr/include/bits /usr/include/asm /usr/include/asm-generic /usr/include/gnu /usr/include/boost)
  def get_match(fulldir, parentdir)
    return nil if SYSTEM_DIR.reduce(nil) { |a, e| a || fulldir.start_with?(e) }
    fulldir.start_with?(parentdir) ? [fulldir, parentdir] : nil
  end

  def match_include(dir)
    @opts[:inc].reduce(nil) { |a, e| a || get_match(dir, e) }
  end

  def store(file)
    File.new(file, 'w').puts JSON.generate(@depend)
  end
end
