#!/bin/env ruby

# To write collect the dependency infomation and write a dotscript
class GenDot
  attr_reader :stdext

  STDSTY = ' [shape=square,color=blue,fontcolor=blue];'
  CCLSTY = '[color=blue]'

  public

  def initialize(scriptname, path)
    @file = File.new('dotscript', 'w')
    @stdext = []
    @path = File.expand_path(path)
  end

  def write(wtcc, wtstd)
    (@wtc, @wtst) = [wtcc, wtstd]
    @file.puts("#!/bin/dot\ndigraph Depend {\n")
    (Dir.glob("#{@path}/*") << "#{@path}/").each { |path| writecontent(path) }
    @file.puts('}')
    @file.close
  end

  private

  def available_dir?(path, spath)
    spath != '.' && spath != '..' && File.directory?("#{path}/#{spath}")
  end

  def pick(path, spath)
    (hname, ccname,fpath) = ["#{spath}.h", "#{spath}.cc", "#{path}/#{spath}"]
    return unless available_dir?(path, spath)
    return [hname, ccname] if File.file?("#{fpath}/#{ccname}")
    return [hname] if File.file?("#{fpath}/#{hname}")
  end

  def writecontent(path)
    return unless File.directory?(path)
    Dir.foreach(path).reduce([]) { |a, e| a << pick(path, e) }.compact
    .each do |fname|
      (hname, cname) = fname.map { |x| x.gsub(/(.cc|.h)$/, '') }
      (fhname, fcname) =
        fname.map { |x| "#{path}/#{x.gsub(/(.cc|.h)$/, '')}/#{x}" }
      File.new(fhname).each { |ln| pitem(hname, ln) }
      File.new(fcname).each { |ln| pitem(cname, ln, CCLSTY) } if @wtc && cname
    end
  end

  def stdput(stdn, fn, lsty)
    @stdext << stdn && @file.puts(stdn + STDSTY) unless @stdext.index(stdn)
    @file.puts("#{stdn} -> #{fn} #{lsty}")
  end

  def pitem(name, line, linestyle = '')
    return unless /#include(?:<(?<std>\w+)>|"(?<nstd>\w+).h")/ =~ line
    @file.puts("#{nstd} -> #{name} #{linestyle};") if nstd && nstd != name
    stdput(std, name, linestyle) if @wtst && std
  end
end

(withcc, withstd) = [nil, nil]
ARGV.each do |item|
  withcc = true if item == '-cc' 
  withstd = true if item == '-std'
end

gd = GenDot.new('dotscript', './')
gd.write(withcc, withstd)
`dot -Teps dotscript -o dependency.eps`
#`rm dotscript`
