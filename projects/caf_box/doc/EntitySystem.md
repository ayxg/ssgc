# CoreApp Framework : Entity System Overview

### Virtual Interface Overloads
- Any base virtual method recieving `self_ent` argument, will be passed owning `Ent` of the contained 
`T`(this). The `self_ent` guaranteed to be cast-able to `T` unchecked within this method by calling
`self_end.As<T>()`.

| Method | Description |
| --- | --- |
| `tCreate(Ent& self_ent)` | Called after the base Ent constructor of T as an Entity. |
| `void tDestroy(Ent& self_ent)` | Called right before destruction of T as an Entity. |
| `eProcRes tProcOsEvent(const OsEventType& ev)` | Called when an OS event is received from a parent entity which is an OS event generator (usually a window). |
| `eProcRes tProcEvents(Ent& self_ent, EventBufferType& evs)` | Called to process the context frame's event buffer for this entity. |
| `eProcRes tProcFrame(Ent& self_ent, const TimeType& dt)` | Called to process the context frame's systems for this entity. |
| `eProcRes tProcGraphics(Ent& self_ent)` | Called to process the context frame's graphics for this entity. |
| `bool tPollOsEvent(OsEventType& ev)` | Disabled by default. Called when an entity is an event generator to create an event. |

