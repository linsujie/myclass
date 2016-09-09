#!/bin/env ruby

require '~/.scripts/gnuplot/plotutils.rb'
require 'rainbow/ext/string'
include PlotUtils

def getdataset(title, value)
  Gnuplot::DataSet.new(value) { |ds| ds.with, ds.title = "error", title }
end

data = PlotUtils::readexpdata(ARGV[0])
keys = data.each_key.to_a

puts "Please select those data you want to display (choose numbers number, or just print 'a' for all of them):".bright
puts keys.each_with_index.map { |k, i| [i, k].join('. ') }.join("\n")
choice = $stdin.gets.chomp!
choice = choice == 'a' ? (0..keys.size - 1).to_a : choice.split(' ').map(&:to_i)

keys = choice.map { |i| keys[i] }
DAT = data.select { |k, _| keys.include?(k) }

Gnuplot.open do |gp|
  gp << "set terminal epscairo enhanced color dashed size 5.5, 4 font 'Helvatica,12' dl 2" << "\n"
  Gnuplot::Plot.new(gp) do |plot|
    plot.output "data.eps"
    plot.logscale 'x'
    plot.xlabel 'E_k(GeV/nucleon)'
    plot.ytics "in format '10^{%T}'"
    plot.xtics "in format '10^{%T}'"
    plot.logscale 'xy'

    plot.data = DAT.map { |k, v| getdataset(k, v) }
  end
end
