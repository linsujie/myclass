#!/bin/env ruby
# encoding: utf-8

require 'fileutils'
require 'rainbow/ext/string'
require 'json'

# A collector to collect the dependecy of all the objects automatically.
class DependCollector
  attr_accessor :depend

  DEFAULT_OPTS = { cc: 'g++', cflag: '-std=c++0x', inc: '' }

  def initialize(file, opts)
    @depend = {}
    @opts = DEFAULT_OPTS.map { |k, v| opts[k] ? [k, opts[k]] : [k, v] }.to_h
    @opts[:inc] = @opts[:inc].split(' ').reject { |x| x == '-I' }
      .map { |x| reject_i(x) }
    @opts[:flag] = @opts[:inc].select { |x| x[0] == '-' }
    @opts[:inc].reject! { |x| x[0] == '-' }

    ObjectSpace.define_finalizer(self,  proc { store(file) })

    return unless File.exist?(file) && !File.zero?(file)
    @depend.merge!(JSON.parse(File.new(file).read))
  end

  def list_inc(obj)
    inc = list(obj).select { |x| File.extname(x) == '.h' }
      .map { |f| File.dirname(f) }.uniq.select { |x| inside_inc(x) }
      .map { |f| "-I #{f}" }
    (inc + @opts[:flag]).join(' ')
  end

  def list(obj)
    @depend[obj.to_s] || gen_depend(obj) || raise("Dependency of file #{obj} error")
  end

  def gen_depend(obj)
    source = obj.sub('.o', '.cc')

    puts "Generating dependency for #{source}".color(:yellow)
    res = `#{@opts[:cc]} #{@opts[:cflag]} #{@opts[:inc].map { |x| "-I #{x}" }.join(' ')} -E -M -fPIC #{source}`
       .split(' ').select { |x| x != '\\' }

    return if res.empty?
    @depend.store(obj.to_s, res[2..-1].sort.unshift(res[1]))
  end

  private

  def reject_i(dir)
    dir.sub(/^-I/, '')
  end

  def inside_inc(dir)
    @opts[:inc].include?(dir)
  end

  def store(file)
    File.new(file, 'w').puts JSON.generate(@depend)
  end
end
