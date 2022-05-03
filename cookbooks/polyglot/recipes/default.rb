ubuntu_mirror = 'http://ubuntu.mirror.rafal.ca/ubuntu/'
ubuntu_release = 'bionic'
ubuntu_version = '18.04'
username = 'vagrant'
user_home = '/home/' + username
project_home = user_home + '/project/' # you may need to change the working directory to match your project


python3_packages = '/usr/local/lib/python3.6/dist-packages'
ruby_gems = '/var/lib/gems/2.5.0/gems/'


# Get Ubuntu sources set up and packages up to date.

template '/etc/apt/sources.list' do
  variables(
    :mirror => ubuntu_mirror,
    :release => ubuntu_release
  )
  notifies :run, 'execute[apt-get update]', :immediately
end
execute 'apt-get update' do
  action :nothing
end
execute 'apt-get upgrade' do
  command 'apt-get dist-upgrade -y'
  only_if 'apt list --upgradeable | grep -q upgradable'
end
directory '/opt'
directory '/opt/installers'


# Basic packages many of us probably want. Includes gcc C and C++ compilers.
package ['build-essential', 'cmake']


# Core language tools
package ['python3', 'python3-pip', 'python3-dev']  # Python
package 'clang' # Clang C/C++ compiler
package 'swig' # SWIG

# Install flask python library
execute 'pip3 install flask'

# Build C programs
execute 'make' do
  cwd project_home + 'server/'
  user username
  environment 'HOME' => user_home
end

# Start the server. For some reason this doesn't work
# execute 'sudo python3 flaskServer.py &' do
#   cwd project_home + 'server/'
#   user username
#   environment 'HOME' => user_home
# end
