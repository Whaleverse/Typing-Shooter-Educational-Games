# Installs Box2D
cd Box2D
if [ "$(uname)" == "Darwin" ]; then
  ../TyperShooter/Resources/premake/premake5-mac gmake
else
  ../TyperShooter/Resources/premake/premake5 gmake
fi
make -C Build clean
make -C Build Box2D

