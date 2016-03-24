#!/bin/env ruby
# encoding: utf-8

require 'fileutils'
require 'pathname'

=begin
 A collector to collect the classes information for those organized in the following way:
           A/A.h [A/A.cc A/A_data A/enumdef]
     or
          submod/B/B.h [submod/B/B.cc submod/B/B_data submod/B/enumdef]
=end
class ClassesCollector
  attr_reader :class

  def initialize(dir)
    @class = {}
    @paths = Pathname(dir).children.select { |c| c.directory? }
    @paths.each { |d| store_class(d) if available?(d) }
    @paths.each { |d| store_subclass(d) }
  end

  def incs
    @class.map { |_, v| "-I #{v[:dir]}" }.join(' ')
  end

  def select(symbol)
    @class.map { |_, v| v[symbol] }.compact.flatten
  end

  def datdir(prefix, cls)
    return unless parent_data_dir(cls)
    %(-D DATDIR=\\"#{prefix}/lib/#{File.basename(parent_data_dir(cls))}\\")
  end

  def index(object)
    result = @class.select { |_, v| v[:o] == object }
    result.empty? ? nil : result[0][0]
  end

  def exclude(arrays)
    arrays.each { |e| @class.reject! { |k, _| k == e.to_sym } }
  end

  def empty?
    @class.empty?
  end

  private

  def store_subclass(dir)
    subcollector = ClassesCollector.new(dir)
    @class.merge!(subcollector.class) unless subcollector.empty?
  end

  def store_class(dir)
    base = dir.basename

    h = ['enumdef', "#{base}_data", "#{base}.h", "#{base}.cc", '']
      .map { |x| dir + x }.zip(%w(def parentdata h cc dir).map(&:to_sym))
      .map(&:reverse).select { |_, v| v.exist? }.to_h

    h[:data] = unfold_dir(h[:parentdata]).map { |d| formdir(d, dir) } if h[:parentdata]
    h[:o] = dir + "#{base}.o" if h[:cc]

    @class[base.to_path.to_sym] = h
  end

  def parent_data_dir(cls)
    @class[cls] && @class[cls][:parentdata]
  end

  def formdir(dir, parent)
    { origin: dir, target: dir.relative_path_from(parent) }
  end

  def unfold_dir(dir)
    subs = dir.children.select { |c| c.directory? }
    subs.empty? ? [dir] : subs.map { |d| unfold_dir(d) }.flatten
  end

  def available?(dir)
    (dir + "#{dir.basename}.h").exist?
  end
end
