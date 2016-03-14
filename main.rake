require 'fileutils'
require 'rake/clean'
require 'rubygems'
require 'json'
require 'rainbow/ext/string'

WITH_GALP = File.exist?(GALPWRAPPER_DIR)
load '~/.galprop/galpdepend/galpdepend.rb' if WITH_GALP

def name(x, post = :o)
  "#{x}/#{File.basename(x)}.#{post}"
end

def getcls(x)
  x.sub(%r{(\w+)/.+}, '\1')
end

def getinc(cls)
  (DEPENDS[name(cls)] or return("-I #{cls}"))[1..-1]
  .map { |x| "-I #{File.dirname(x)}" }.join(' ')
end

def sys(str, info)
  puts VERBOSE == :detail ? str : info
  system(str)
end

DATALIST = %w(dm_production/anaspec dm_production/anaspec_pppc dm_production/anaspec_zhou load_dat pp_antiproton)

FLIST = FileList['*', '*/*'].select { |x| File.exist?(name(x, :h)) } - EXCLUDE
CLIST = FileList['*', '*/*'].select { |x| File.exist?(name(x, :cc)) } - EXCLUDE
HLIST = FLIST - CLIST

WLIST = FileList[%w(chi2prop mcparas propagator create_source dm_production/anaspec_zhou)]
LLIST = CLIST - WLIST

WINC, LINC = FLIST, LLIST + HLIST
INC = WINC.map { |x| "-I #{x}" }

CLEAN.include(CLIST.map { |x| name(x) })
CLEAN.include(FLIST.map { |x| name(x, 'h.gch') })

OBJS = { work: WLIST.map { |x| name(x) }, lin: LLIST.map { |x| name(x) } }

TASKLIST = FileList[:lin, :work]
target = ->(t) { STATIC ? "lib#{t}s.a" : "lib#{t}.so" }

CLOBBER.include(TASKLIST.map { |x| target.call(x) })

task default: TASKLIST

COLLEC = STATIC ? 'ar crs' : "#{CC} #{CFLAG} -shared -fPIC -o"

DEPNAME = 'DEPENDENCY'
def gendepend(srccls)
  puts "Generating depends for #{srccls}"
  res = `#{CC} #{CFLAG} #{INC} #{WITH_GALP ? DEPEND.inc_to_s : ''} -E -MM -fPIC #{name(srccls, :cc)}`
       .split(' ').select { |x| x != '\\' }
       #.map { |x| x.sub(/[.]h$/, '.h.gch') } # for precompiling
  [name(srccls), res[1..-1]]
end

NO_DEP_WARN = 'WARNNING::'.bright + \
  "There is no dependency file, please execute:\n".color(:white) + \
  "   rake dep\n" + 'to generate it'.color(:white)

def readdepend
  depend_exist = File.exist?(DEPNAME) && File.size(DEPNAME) != 0
  depend_exist ? JSON.parse(File.new(DEPNAME).read) : puts(NO_DEP_WARN)
end

DEPENDS = readdepend

TASKLIST.each do |grp|
  desc "Generating lib#{grp}"
  multitask "#{grp}" => OBJS[grp] do
    targ = target.call(grp)
    sys("#{COLLEC} #{targ} #{OBJS[grp].to_s}", "Link to #{targ}".bright)
  end
end

desc 'Generating (refresh) the dependency for all the classes'
task :dep do
  file = File.new(DEPNAME, 'w')
  file.puts JSON.generate(Hash[CLIST.map { |x| gendepend(x) }])
  file.close
end

desc 'Installing the libraries'
task :install do
  incfile = WINC.map { |x| "#{x}/#{File.basename(x)}.h" }.to_s

  ins = [[incfile, "#{PREFIX}/include"]] + \
    [[Dir.glob('*/enumdef/*def').join(' '), "#{PREFIX}/include/enumdef"]] + \
    [[Dir.glob('*/*/enumdef/*def').join(' '), "#{PREFIX}/include/enumdef"]] + \
    CLOBBER.map { |x| File.exist?(x) && [x, "#{PREFIX}/lib"] } + \
    DATALIST.map { |c| ["#{c}/#{File.basename(c)}_data/*", "#{PREFIX}/lib/#{c}_data"] }

  ins.select { |x| x }.each do |o, t|
    FileUtils.mkdir_p(t)
    sys("install -D #{o} #{t}", "Installing:".bright + " #{o}")
  end
end

def compile(cls, t, create = nil)
  dbg = DLIST.include?(cls) ? DEBUG : nil

  datdir = %Q(-D DATDIR=\\"#{PREFIX}/lib/#{cls}_data\\")
  dat = DATALIST.include?(cls) ? datdir : ''

  cmd = [CC, CFLAG, dbg, dat, WITH_GALP ? "-D GALP_#{GALPVERSION}" : '',
         getinc(cls), WITH_GALP ? DEPEND.inc_to_s : '',
         create, '-fPIC', t.source, '-o', t.name].join(' ')

  sys(cmd, "Compiling #{t.name}")
end

CLIST.each do |cls|
  break unless DEPENDS
  file name(cls) => DEPENDS[name(cls)] do |t|
    raise 'libwork depend on galpwrapper'.bright if !WITH_GALP && OBJS[:work].include?(t.name)
    compile(cls, t, '-c')
  end
end
