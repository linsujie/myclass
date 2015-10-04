#!/bin/env ruby

require '~/.scripts/gnuplot/plotutils.rb'
include PlotUtils

def getdataset(title, value)
  Gnuplot::DataSet.new(value) { |ds| ds.with, ds.title = "error", title }
end

DAT = PlotUtils::readexpdata(ARGV[0])

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
