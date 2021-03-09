# Gitlab CI
This pipeline is configured mostly to run on master. At the moment the build stage only runs on master because of the artifactory pushes in that stage.

## Running on branches
If pipelines should be run on branches, make sure to make a trivial change (a comment would do) to the docker files so that the pipeline will build new images in the container registry for the branch. The docker stage will only run when changes are made a docker file.

## Adding new stages
To add a new stage, make a new file for that stage in the format ```<stage>.gitlab-ci.yml``` and add it the stages and include structure in the top level ```.gitlab-ci.yml```.