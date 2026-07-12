# Common hardware building blocks

`hw/common/` contains reusable clock, reset, CDC, buffering, and edge-detection modules used across the SoC. The canonical filelist is [`_common.f`](_common.f).

Use the synchronizer and asynchronous-buffer modules for clock-domain crossings rather than introducing ad-hoc CDC logic in peripherals. Any new reusable primitive should be added to the filelist and documented here.
