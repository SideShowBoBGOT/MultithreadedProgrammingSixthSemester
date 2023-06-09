package org.LabMath.Interfaces.General;

import jdk.jshell.spi.ExecutionControl;

public interface Divisible<T> {
    void div(T other) throws ExecutionControl.NotImplementedException;
}
