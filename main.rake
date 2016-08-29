require 'fileutils'
require 'rake/clean'
require 'rubygems'
require 'json'
require 'rainbow/ext/string'
require_relative 'classescollector'
require_relative 'dependcollector'

WITH_GALP = GALPWRAPPER_DIR && File.exist?(GALPWRAPPER_DIR)
load '~/.galprop/galpdepend/galpdepend.rb' if WITH_GALP

def sys(str, info)
  puts VERBOSE == :detail ? str : info || str
  system(str)
end

def target(t)
  STATIC ? "lib#{t}s.a" : "lib#{t}.so"
end

CLASSES = ClassesCollector.new('.')
CLASSES.exclude(EXCLUDE)

DEPENDENCY = DependCollector.new('DEPENDENCY', cc: CC,
                                 cflag: ["#{CFLAG} #{WITH_GALP ? "-D GALP_#{GALPVERSION}" : ''}",
                                         FLAGS.map { |f| "-D #{f}" }].join(' '),
                                 inc: [CLASSES.incs, WITH_GALP ? DEPEND.inc_to_s : ''].join(' '))

WLIST = %w(chi2prop mcparas anaspec_zhou)
LIST = { work: WLIST.reject { |x| EXCLUDE.include?(x) }.map(&:to_sym) }
LIST[:lin] = CLASSES.class.each_key.to_a - LIST[:work]

TASKLIST = FileList[:lin, :work]

CLEAN.include(CLASSES.select(:o))
CLOBBER.include(TASKLIST.map { |x| target(x) })

task default: TASKLIST

COLLEC = STATIC ? 'ar crs' : "#{CC} #{CFLAG} -shared -fPIC -o"

TASKLIST.each do |grp|
  desc "Generating lib#{grp}"
  multitask "#{grp}" => LIST[grp].map { |c| CLASSES.class[c][:o] }.compact do |t|
    sys("#{COLLEC} #{target(grp)} #{t.sources.join(' ')}",
        "Link to #{target(grp)}".bright)
  end
end

def compile(t)
  taskname = File.basename(t.name).sub('.o', '')

  sys([CC, DEPENDENCY.cflag,
       STATIC ? nil : '-shared -fPIC',
       DLIST.include?(taskname) ? DEBUG : '',
       FLAGS.map { |f| "-D #{f}" }.join(' '),
       CLASSES.datdir(PREFIX, taskname.to_sym),
       '-c -o', t.name,
       DEPENDENCY.list_inc(t.name),
       t.source].join(' '), "Compilling ".bright + t.name)
end

CLASSES.class.each do |cls, paths|
  next unless paths[:o]

  begin
    DEPENDENCY.list(paths[:o])
  rescue RuntimeError => e
    puts "Warnning: #{e}, it may not be linked into the library".color(:red)
    next
  end

  file paths[:o] => DEPENDENCY.list(paths[:o]) do |t|
    raise 'libwork depend on galpwrapper'.bright if !WITH_GALP && LIST[:work].include?(cls)
    next if compile(t)
    puts "The exist dependency incorrect, now try to regenerate it".color(:blue)
    DEPENDENCY.gen_depend(t.name)
    compile(t)
  end
end

desc 'Installing the libraries'
task :install do
  ins = [[CLASSES.select(:h).join(' '), "#{PREFIX}/include"]] + \
    CLASSES.select(:def).map { |d| ["#{d}/*", "#{PREFIX}/include/enumdef" ] } + \
    CLASSES.select(:data).map { |d| ["#{d[:origin]}/*", "#{PREFIX}/lib/#{d[:target]}"] } + \
    CLOBBER.map { |x| File.exist?(x) && [x, "#{PREFIX}/lib"] }.compact

  ins.select { |x| x }.each do |o, t|
    FileUtils.mkdir_p(t)
    sys("install -D #{o} #{t}",
        "Installing: ".bright + (o =~ /(.so$|.a$)/ ? o.bright : o))
  end
end
