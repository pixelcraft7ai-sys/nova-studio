# 0005 — Module layering, enforced by a test
Status: Accepted
```
core  <-  document  <-  render, formats        (no Qt)
core  <-  platform                              (Qt Core only)
app   ->  all                                   (only module with QtWidgets)
```
`tests/scripts/check_layering.cmake` fails the build if `core/document/render/formats` include Qt, or a lower layer includes a higher one; `platform` may not include QtWidgets/QtGui. A self-test proves the checker catches violations. `MainWindow` owns no document and does no image processing; it receives `AppServices`.
**Limits.** The check is include-based (textual); it does not catch link-level leaks. Names may change with a documented reason.
