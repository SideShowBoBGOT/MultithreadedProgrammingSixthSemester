package LabMath.Interfaces.General;

import jdk.jshell.spi.ExecutionControl;

public interface GetMultipliable<T> {
   T getMul(T other) throws ExecutionControl.NotImplementedException;
}
