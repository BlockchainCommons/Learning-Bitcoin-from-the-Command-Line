# Translating Learning Bitcoin from the Command Line

Thank you for your interest in translating Learning Bitcoin from the Command Line! What follows is our standard procedure for doing so. If you have any questions, please file an Issue.

1. **Please Follow Our Normal Contributor Procedure.**
   * Read over [CONTRIBUTING.md](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/CONTRIBUTING.md).
   * Fill out a [Contributor License Agreement](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/blob/master/CLA.md), sign it with GPG, and submit it as a PR. Be sure this is done for everyone who contributes to the translation.
1. **Choose a Release.**  
   * Choose a [Release](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/releases) as the basis of your translation. We generally suggest the latest release. This will ensure the consistency of all the files in your translation, will insulate you from any changes we make, and will make it easy to see what has changed when we create a new release. 
   * If it looks like there hasn't been a new Release in a while, file an Issue saying you're interested in starting a new translation, and asking if it would make sense for there to be a new Release milestone before you do so. If there's been anything notable, and we're not in the middle of things, we'll likely create a new patch or minor version. If we're in the middle of things, we'll just suggest you use the previous Release.
   * Label your table of contents and each chapter or section with the release used.
1. **Request a Branch.**
   * File an [Issue](https://github.com/BlockchainCommons/Learning-Bitcoin-from-the-Command-Line/issues) requesting a new branch for your translation.
   * This will be the master place for us to collect work on the translation over time.
   * We will create a top-level directory for your complete translation using the [ISO 639-1 language code](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes), for example `es` (Spanish), `fr` (French), or `pt` (Portuguese). Work should be done in that directory.
1. **Fork the Branch.**
   * Once we've created a translation branch, you'll then want to fork that into your own GitHub account.
   * Generally, we suggest that you create one working branch off that translation branch for each separate chapter. This will allow you to work through the process of write/review/revise for each individual chapter without it getting tangled up with your new content for future chapters, and will allow us to merge the chapters as they're completed, which is our preference, and will help everyone to keep track of where things our.
   * Make sure you [sign your commits](https://docs.github.com/en/github/authenticating-to-github/managing-commit-signature-verification/signing-commits).
1. **Submit PRs a Chapter at a Time.**
   * Submit your PRs for the translation from your working fork to our translation branch in batches of no more than a single chapter at a time.
   * Submit in smaller batches if it makes sense, for example because different people are writing different sections.
   * Again, we suggest that there be a branch for each chapter, so when you submit your PR for one chapter, you'll usually then create a branch for the next chapter.
1. **Request Approval from a Native Speaker.**
   * No one can ever do a great edit of their own work, so we require each section to be approved by someone other than the original translator.

   * This fundamentally means that any translation team _should_ contain at least two members, either one translator and one editor or else two people who trade off roles of translator and editor. If your team doesn't have a second member, we can put out a call for an editor/approver when you submit a PR, but it's possible that we won't be able to find one, and your hard work will languish, so it's s better to have one up front.

   * To make the editing and reviewing process easier, independent, and streamlined, we suggest that you and the reviewer [request access to each other's forks](https://docs.github.com/en/enterprise-server@3.0/github/setting-up-and-managing-your-github-user-account/managing-access-to-your-personal-repositories/inviting-collaborators-to-a-personal-repository). Once invited, the _reviewer_ can then:

     1. Easily checkout the PR into their local machine with [GitHub CLI](https://cli.github.com/) by doing `gh pr checkout <pr-number>`. This will automatically create a branch with the PR code for the reviewer to work on.

     2. Then, perform their changes with signed commits.

     3. And do `git push` after finishing the review to have the changes reflected in the PR.
1. **Request Approval from the Blockchain Commons Team.**
   * Once a chapter or section has been approved by a native speaker, request approval from someone on the Blockchain Commons team: currently [@shannona](https://github.com/shannona).
1. **Continue!**
   * Continue through the process, no more than one chapter at a time, until you have a full book.
   * Be aware of the scope of the overall project. As of v2.01, Learning Bitcoin from the Command Line is 120,000 words in length. As a book, that'd be 250-400 pages, depending on the format and layout. (About 90,000 words of that is text to translate, with the remainder being code.) You want to make sure you have the time for that level of commitment before getting started.
1. **Let Us Know When You're Done.**
   * When you've completed your translation, file an issue to let us know that the translation branch is ready to be merged into the master.
   * This will also let us know to announce the completed translation and link it into the master README
1. **Update Your Translation with New Releases**
   * It is our hope that translations will stay up to day with new releases, particularly major and minor releases, which are likely to include new content and updates. Currently, these only occur ever few years
   * If you have decided to stop updating a translation, please let us know in an Issue, so that we can let the community know that we are looking for a new translator to continue updating a translation.

### WORKFLOW EXAMPLE

Chapter 3 has 7 subsections: 3.0, 3.1, 3.2, 3.3, Interlude 3.3, 3.4 and 3.5.
Suppose you want to translate it to french.

One approach to its translation would be to create a branch `french-3` from
`french-translation` branch (created in the previous step). Then create a
translation file for the 3.0 subsection. When you think that chapter 3.0 is
ready for review you commit your changes and continue with the next file,
subsection 3.1. Repeat the same process for this file and all the following.
After this, your branch `french-3` will diverge from the base branch
(`french-translation` in this case) by 7 different commits, one for each
subsection, plus some more commits if you forgot to make some changes or need
to correct others.  At this point you are ready to push your changes to your
fork and open a pull request to the original repository.

In the review process you will discuss some decisions with your reviewer and
introduce new changes in the form of commits if you have to change some parts
of your modifications.  Don't forget to commit those changes in meaningful
chunks and push them to your fork.  After polishing your changes, your reviewer
will accept your changes and it will request the merge of those changes on the
`french-translation` branch.  Note that you can also push your changes and
create your pull request after your first commit. This will allow the reviewer
to start reviewing before you finish all chapter translation.

You can track the state of the translation of that particular chapter in the 
body of the pull request, with the following template:

```
This pull request creates the <translated_language> version of the chapter
<chapter_number> of Learning-Bitcoin-from-the-Command-Line.

Things to check:
- Respect filenames and chapter names from translated [README](local/path/to/README).
- Correct translation.
- Term consistency: review ["translation memory"](local/path/to/file) file.
- Good punctuation.
- Correct linking.
- Nice formatting.

| Sub chapter   |       Translated        |         Reviewed        |          Done           |
| ------------- |   :--------------:      |    :--------------:     |    :--------------:     |
| _0_           | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
| _1_           | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
| _2_           | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
| _3_           | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
| _etc_         | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> | <ul><li> [ ] </li></ul> |
```

### FINAL SUGGESTIONS:
- _Keep lines under 80 characters_. Markdown parser will format them right
  (double check that) and the review process will be easier.
- _Translate `README.md` file first_ and use it as reference for the translation
  of all the following chapter names, filenames and link names.  The README
  represents the state of the master branch at the moment of begin the
  translation process. It will be consulted by all translators to know how to
  translate file and chapter names. Try to reach an agreement with the other
  translators about those names before writing any chapter and keep further
  changes of this file to minimum.
- _Create a "translation memory" file_ with special words that could have
  multiple translations under the same contexts and the translation chosen for
  them. Good candidates to be in this list would be domain specific words,
  like: multisig, input, output, script, timelock, locktime, hash, etc.


Again, thank you!
