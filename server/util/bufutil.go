package util

import "io"

func ReadString(buf io.Reader) (string, error) {
	data, err := ReadBytes(buf)
	if err != nil {
		return "", err
	}

	return string(data), nil
}

func ReadBytes(buf io.Reader) ([]byte, error) {
	length, err := ReadUnsignedShort(buf)
	if err != nil {
		return nil, err
	}

	data := make([]byte, length)
	_, err = buf.Read(data)
	return data, err
}

func ReadUnsignedShort(buf io.Reader) (uint16, error) {
	data := make([]byte, 2)
	_, err := buf.Read(data)
	return (uint16(data[0]) << 8) | uint16(data[1]), err
}

func ReadByte(buf io.Reader) (byte, error) {
	data := make([]byte, 1)
	_, err := buf.Read(data)
	return data[0], err
}

func WriteString(buf io.Writer, value string) error {
	return WriteBytes(buf, []byte(value))
}

func WriteBytes(buf io.Writer, value []byte) error {
	err := WriteUnsignedShort(buf, uint16(len(value)))
	if err != nil {
		return err
	}

	_, err = buf.Write(value)
	return err
}

func WriteUnsignedShort(buf io.Writer, value uint16) error {
	_, err := buf.Write([]byte{ byte(value >> 8), byte(value)})
	return err
}

func WriteByte(buf io.Writer, value byte) error {
	_, err := buf.Write([]byte{value})
	return err
}
