var gulp = require('gulp')
var ghPages = require('gulp-gh-pages')
var add = require('gulp-add')
var exec = require('child_process').exec

var config = {
  push: true
}

function deployMarkers() {
  return {
    'UPDATED_AT': new Date().toISOString(),
    '.nojekyll': ""
  }
}

gulp.task('build', function(done) {
  exec('make docs', function(err, stdout, stderr) {
    if (err) throw err
    process.stdout.write(stdout)
    process.stderr.write(stderr)
    done()
  })
})

gulp.task('deploy', ['build'], function() {
  return gulp.src('docs/build/**/*')
    .pipe(add(deployMarkers()))
    .pipe(ghPages(config))
})
