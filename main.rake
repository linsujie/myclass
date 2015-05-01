require 'fileutils'
require 'rake/clean'

def name(x, post = :o)
  "#{x}/#{x}.#{post}"
end

def sys(str)
  puts str
  system(str)
end

DATALIST = %w(anaspec anaspec_pppc load_dat)

FLIST =  FileList['*'].select { |x| File.exist?(name(x, :h)) }
CLIST = FileList['*'].select { |x| File.exist?(name(x, :cc)) }
HLIST = FLIST - CLIST

WLIST = FileList[%w(chi2prop mcparas propagator create_source)]
LLIST = CLIST - WLIST

WINC, LINC = FLIST, LLIST + HLIST

CLEAN.include(CLIST.map { |x| name(x) })

OBJS = { work: WLIST.map { |x| name(x) }, lin: LLIST.map { |x| name(x) } }

TASKLIST = FileList[:lin, :work]
target = ->(t) { STATIC ? "lib#{t}s.a" : "lib#{t}.so" }

CLOBBER.include(TASKLIST.map { |x| target.call(x) })

task default: TASKLIST

COLLEC = STATIC ? 'ar crs' : "#{CC} #{CFLAG} -shared -fPIC -o"

TASKLIST.each do |grp|
  desc "Generating lib#{grp}"
  multitask "#{grp}" => OBJS[grp] do
    sys("#{COLLEC} #{target.call(grp)} #{OBJS[grp].to_s}")
  end
end

desc 'Installing the libraries'
task :install do
  incfile = WINC.map { |x| "#{x}/#{x}.h" }.to_s

  ins = [[incfile, "#{PREFIX}/include"]] + \
    CLOBBER.map { |x| File.exist?(x) && [x, "#{PREFIX}/lib"] } + \
    DATALIST.map { |c| ["#{c}/#{c}_data/*", "#{PREFIX}/lib/#{c}_data"] }

  ins.select { |x| x }.each do |o, t|
    FileUtils.mkdir_p(t)
    sys("install -D #{o} #{t}")
  end
end

rule '.o' => '.cc' do |t|
  cls = t.name.sub(%r{\w+/(\w+).o$}, '\1')
  dbg = DLIST.include?(cls) ? DEBUG : nil
  datdir = %Q(-D DATDIR=\\"#{PREFIX}/lib/#{cls}_data\\")
  dat = DATALIST.include?(cls) ? datdir : ''

  inc = WINC.map { |x| "-I #{x}" }
  sys("#{CC} #{CFLAG} #{dbg} #{dat} #{inc} -c -fPIC #{t.source} -o #{t.name}")
end
