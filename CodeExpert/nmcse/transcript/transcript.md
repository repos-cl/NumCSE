## Transcript of the communication with the CodeExpert team

A *[...]* indicates an omitted part.

### 2022/03/17 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...] I am working for Prof. Ralf Hiptmair on the NumCSE course and we are currently rethinking our build process. We have decided that we would like to update both Eigen to 3.4 and the C++ std in our own repository, but naturally, we need to make sure this is compatible with CE in order for students being able to reproduce our codes. Are these updates possible? The CE “expert” in our team told me I would have to contact you (the admins) about this, because Eigen was centrally installed. [...]

###  2022/03/17 [David Sichau](expert@inf.ethz.ch):

[...]

I think you are still using the generic 1 image. As this is based on the outdated redhat-7 we will not make any changes to this image.

We have an generic 2 image which is based on redhat-8 but did not work in some of your use cases.

I think this update might be useful to make the switch to generic 2. We can help you in this process. But the testing of this image must happen on your side.

I gave you access to our environments:
https://gitlab.inf.ethz.ch/OU-LECTURERS/containers/cxenv

There you can have a look at generic-2 image. Please let us know what you actually need to be installed.

For more details on our execution please have a look at our documentation:
https://docs.expert.ethz.ch/lecturers/#runner-and-cxenvironments

[...]

###  2022/03/18 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

thank you for your help. I did some investigating about our whole cx workflow, we had an internal talk and it turns out that we have two major issues. I am telling you these, because I believe it is better to give you the big picture.

1. Stable build environments

We have a very large repository of codes maintained by a very heterogeneous team (in terms of machines used in development), thus we are constantly running into build and compatibility issues. The assignment codes that are later uploaded to cx are a part of that repository and they are maintained and developed „offline“, i.e. independent of cx on our machines. To eliminate that risk we are currently working on a unified docker solution to solve all these problems. Since the assignment codes are tested and developed in that environment, it would only make sense that we use this (or some easy-to-maintain mirror of this) container for cx as well. I suggest we develop a custom image / cxEnvironment for our course, (I see this has already been done for some other course called „ifme1“). The drawback is that the NumCSE team will have to maintain the image.

2. Upload automation

Since the exercises have been developed over years and each year only a small subset of assignments are published on cx, we would like to have an automated (in the sense that one cannot make crucial mistakes), scalable way of uploading our codes as a whole project, such that the work necessary in cx itself is limited to some reasonable amount. We have experimented with browser automation (which worked OK for a while), but ultimately this is a costly and unsustainable decision, once again driving up the month-to-month maintenance work for the course. In my opinion, we are in an unfortunate scenario right now. Do you have any ideas on this? Do other courses have similar issues? I am aware that cx was probably not designed with this kind of workflow in mind. Is it a feasible way to maybe convert our file structure to some flat hierarchy which can be manually bulk-uploaded to cx? What if we .zip the directory?

Here is an example of one assignment (which is then mapped to a cx project) and how it is stored in our repository. Our testing scripts are stored externally  and later added to the cx project.

[...]

###  2022/03/19 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

I just wanted to apologize for bothering you with the "upload automation“ chapter. Our team must have missed the „upload from file“ feature in cx. This completely solves the second issue in the best way I could ever imagine. Sorry…

[...]

###  2022/03/21 [David Sichau](expert@inf.ethz.ch):

[...]

yes I think a custom images is the way to go for your course as you have very special usecases. Do you want to start with our base image or do you want to base it on generic-2? I will then create a special repo for you. As soon as you are happy with the image let us know and we will deploy it to Code Expert.

The automatic upload was developed based on the needs for your course. I am glad that this solves your problems.

[...]

###  2022/03/25 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

I still had to clarify a few issues with our exercise uploading process in the last few days, so I could not yet give you an answer. I think it is better to start fresh with your base image to keep the containers clean. As previously mentioned, we would like to use a single, unified image for Code Expert and internal use cases so that our assistants (and sometimes students) will only have to deal with a single container for all purposes. Do you think this can be achieved or is that a bad idea for some reason I cannot see right now? This means that the image will be larger than it has to be for just the Code Expert use case. An example would be development tools like CMake and Clang-tidy.

[...]

###  2022/03/28 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

we've prepared two new repositories for environment images for you:

[1] https://gitlab.inf.ethz.ch/OU-LECTURERS/containers/cxenv/nmcse
[2] https://gitlab.inf.ethz.ch/OU-LECTURERS/containers/cxenv/nmcse-dev

We suggest you set up and use [1] for the actual student jobs and [2] for development, i.e. [2] can include additional development tools you might need, while [1] is kept as lean as possible. 

Currently only you have been assigned maintainer role in gitlab and pull permissions from our registry cxhub.ethz.ch. Pushing to the repositories will trigger an automatic rebuild of the images (including push to the registry), but will not make them available in Code Expert immediately. Please drop us a mail whenever you have a new version of an image ready, so we can update them in Code Expert accordingly. 

Additionally, if you'd like to change the default project associated with these images, please set one up in Code Expert and send us an export (i.e. "Download Project" in our IDE). 

[...]

###  2022/03/28 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

thanks for the setup. I still have to think about the dual repository approach, maybe we will end up basing our dev image on the lean version and just build that part ourselves (but that is still far into the future). To which account/email did you give the pull permissions for cxhub.ethz.ch? I tried two docker accounts registered for heinrich.grattenthaler@sam.math.ethz.ch and heinrich.grattenthaler@inf.ethz.ch, but authentication failed for a pull from cxhub.ethz.ch/cx/cxenv/base-rhel8:latest.

[...]

###  2022/03/28 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

for the moment we think the dual environment approach best suits your use case and our environment. Two repositories are necessary in order to benefit from our automation in the background (we haven't had the need to build multiple environments from one repos so far). You can, of course, base the nmcse-dev image on nmcse. 

As for the permissions: please log in to our registry with your nethz account, i.e. docker login -uhgratten cxhub.ethz.ch

[...]

###  2022/04/03 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

I tried building the nmcse image locally. In instruction 4 ``RUN dnf install -y autoconf automake gcc gcc-c++ bison flex b...``, dnf fails to access the isginf-8 package repository: 

```
#8 132.7 isginf Extra Packages for 8 - x86_64            0.0  B/s |   0  B     01:59    
#8 132.7 Errors during downloading metadata for repository 'isginf-8':
#8 132.7   - Curl error (28): Timeout was reached for http://install-el8.inf.ethz.ch/software/el8/x86_64/repodata/repomd.xml [Connection timed out after 30001 milliseconds]
#8 132.7   - Curl error (28): Timeout was reached for http://install-el8.inf.ethz.ch/software/el8/x86_64/repodata/repomd.xml [Connection timed out after 30000 milliseconds]
#8 132.7   - Curl error (28): Timeout was reached for http://install-el8.inf.ethz.ch/software/el8/x86_64/repodata/repomd.xml [Connection timed out after 30002 milliseconds]
#8 132.7 Error: Failed to download metadata for repo 'isginf-8': Cannot download repomd.xml: Cannot download repodata/repomd.xml: All mirrors were tried
```

For now, I ignored the settings in deploy-docker.cfg, but I assume this is unrelated.
Can you help me out here? Maybe I am not supposed to build these images locally at all and only use the CI build for development?

[...]

###  2022/04/04 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

the repository is maintained by ISG and is only accessible internally or via VPN. 

[...]

###  2022/05/16 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

we have tested our `nmcse` container to a sufficient degree and - for now - happy with its performance (in the simulator). As a real-world test, could you please deploy it on the Code Expert platform?

Also, we will now start development of the `nmcse-dev` container and see whether basing it on the student container is a feasible solution. This would be a great way to avoid redundancy.

[...]

###  2022/05/16 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

the environment is already available on expert.ethz.ch. 

If you'd like a custom template project for this environment, you can send us a project export and we'll set it up for you.

[...]

###  2022/05/17 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

thanks for deploying the container! 

In our simulator, we used `-e MAXFILESIZE=1000000 -e TIMEOUT=1000 -e CPUTIME=1000` for running, although I believe only the first environment variable is necessary. Is it possible for us to modify these variables on the platform or is that something only you can do?

[...]

###  2022/05/17 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

although you can modify the environment variables, it won't have the desired effect - they're informational only and required by some test runners. 

Besides those you mentioned, a few more limits are enforced for your environment (see screenshot below):

![screenshot](E96A2F09-299B-4BE5-9AC3-228CC67BEBAE_1_105_c.jpeg)

These can only be changed by us on request. 

[...]

###  2022/06/03 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]


We did some more testing and it turns out that we require at least 20MB of file size (since one of our tests does not successfully compile otherwise). Could you please raise the file size limit of our container to 20MB? We used MAXFILESIZE=20000 in the simulator.

[...]

###  2022/06/03 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

File size has been increased to 20MB.

[...]

###  2022/08/29 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

Vor einiger Zeit hatten wir die beiden Code Expert Environments nmcse und nmcse-dev für Euch eingerichtet. 
Bislang wurden mit diesen Environments erst sehr wenige (nmcse) bzw. keine (nmcse-dev) Jobs auf expert.ethz.ch ausgeführt. 
Da wir derzeit unseren Buildprozess für Environment Images umstellen, taucht nun die Frage auf, ob beide Environments in Zukunft noch genutzt werden sollen.

Für eine kurze Info wäre ich dankbar.

[...]

###  2022/08/29 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

Ja, das nmcse image wird in Zukunft genutzt. Wir haben dies bewusst im Frühlingssemester entwickelt, da der entsprechende Kurs immer im HS läuft.

Das andere Environment (nmcse-dev) war ursprünglich für Entwicklungsarbeiten gedacht. Im Moment setzen wir da aber auf local builds, da das Image hauptsächlich für das NumCSE-Team von Relevanz ist und wir z.B. auch die aarch64-Architektur nativ unterstützen möchten. Es sieht also im Moment nicht danach aus, als würden wir für dieses Image in Zukunft noch den Buildprozess auf Code Expert benötigen.

[...]

###  2022/08/30 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

Besten Dank für die Antwort. Ich habe soeben das nmcse-dev Environment entfernt und das zugehörige Repo in gitlab archiviert. 

Für nmcse habe ich gestern die Gitlab Pipeline auf unseren neuen Buildprozess umgestellt: 
- das Image wird automatisch gebaut, wenn ein neues git tag gepusht wird oder eine Pipeline via https://gitlab.inf.ethz.ch/OU-LECTURERS/containers/cxenv/nmcse > CI/CD > Run Pipeline über das Gitlab Webinterface gestartet wird.
- alle Environment Images werden täglich automatisch aktualisiert und gebaut, um sie auf einem aktuellen Stand zu halten.
- Environment Images werden immer mit 'latest'  und entweder dem git tag oder dem commit sha getaggt, z.B. cxhub.ethz.ch/cx/cxenv/nmcse:latest und cxhub.ethz.ch/cx/cxenv/nmcse:972285f9. 'latest' entspricht dann immer dem zuletzt gebauten Image.
- derzeit benötigt es noch einen manuellen Schritt, um die Environment Images auch in Code Expert zu aktualisieren. Das soll in Zukunft noch automatisiert werden und Ihr werden das Update dann selbst auslösen können. 

[...]

###  2022/09/17 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

We just encountered some issues with the new build pipeline. The pipeline failed with: Project `OU-LECTURERS/gitlab-templates` not found or access denied! Make sure any includes in the pipeline configuration are correctly defined.  Could you please have a look into it? Since the changes relative to the last working commit are minimal, I assume it is merely a permissions problem.

[...]

###  2022/09/17 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

Sorry for the inconvenience. I've added you to our project that contains the template files.
Please try again and let me know if it works now.

[...]

###  2022/09/19 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

I manually triggered a pipeline but it failed in the build stage. You can find the details in GitLab: https://gitlab.inf.ethz.ch/OU-LECTURERS/containers/cxenv/nmcse/-/jobs/646713. The daily builds triggered by your account have succeeded, though.

Also, for now, could you please deploy the current image to code expert so we can be operational again?

[...]

###  2022/09/19 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

The current image has been deployed on Code Expert. 

We're currently migrating our docker registry to a new server and you happened to push to it right at the moment when it was operating in read-only mode. Sorry for that. 

[...]

###  2022/09/19 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

Just to keep you updated: the new docker registry does not yet work as expected. 
Our ISG is working on it and it should be back to normal operation soon. 
Please let me know if you require any urgent changes to the environment image. 

I'll let you know once the problems are resolved. Sorry again for the inconveniences. 

[...]

###  2022/09/19 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

Pushing to the registry should now be possible again. 
Please let me know if you encounter any further problems.

[...]

###  2022/09/20 [Heinrich Grattenthaler](heinrich.grattenthaler@sam.math.ethz.ch):

[...]

Some time ago we tested our nmcse container as of mid April 2022 with all our assignments from last year and it went through without problems on code expert. We are now struggling to reproduce this stable behavior both locally and on code expert. There are two recurring issues: 

First, the compilation for some of our assignments is killed, but only some of the time. This looks like a memory issue. I.e we observe something of the form `g++: fatal error: Killed signal terminated program cc1plus` and in the next run, it is all fine.

Second, uploading the exact same project with a different stub configured, i.e. `generic-1` instead of `nmcse`, not only speeds up compilation by a factor of 5x, but also runs the job without any (observed) issues. Note that this could always have been the case, as we did not compare the two environments up until recently. For example, observe the difference between the test action for https://expert.ethz.ch/ide2/9svnsbhbGgGWSKdmR (nmcse based) and https://expert.ethz.ch/ide2/vrrzChv7LWZrjoZCQ (generic-1) based.

The only explanation I have for this kind of difference is that our nmcse containers run with some resource constraints that generic-1 containers are not limited by. Could this be the case or did we really just do something fundamentally wrong with our installs? If so, could we have comparable "privileges“ for nmcse instances? 

The way things are right now, I cannot justify using our custom environment for the course and we will have to fall back to generic-1.

[...]

###  2022/09/20 [Stefan Dröschler](expert@inf.ethz.ch):

[...]

your assumption was correct. The limits imposed on nmcse were a little stricter, i.e. 12 sec cpu time instead of 60 sec and 300 MB memory/swap instead of 1000 MB. 

I've adjusted the limits for nmcse and they're equal to generic-1 now.

[...]